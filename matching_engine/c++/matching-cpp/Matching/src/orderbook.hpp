#pragma once

#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <memory>
#include <numeric>
#include <queue>
#include <vector>
#include <string_view>
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include <boost/container/map.hpp>
#include <boost/container/deque.hpp>
#include <boost/container/adaptive_pool.hpp>
#include <boost/container/allocator.hpp>
#include <boost/container/node_allocator.hpp>
#include <boost/align/aligned_allocator.hpp>
#include <boost/align/aligned_delete.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace matching_engine
{

using Price = unsigned long;
using Quantity = double;
using Time = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Time>;
using UUID = boost::uuids::uuid;

enum SIDE { BUY, SELL };

enum STATE { INACTIVE, ACTIVE, CANCELLED, FULFILLED };

enum TIF { GTC };

class Order
{
private:
    const std::string_view market_name_; /* TODO Replace with std::string_view */
    const SIDE side_;
    const Price price_;
    const Quantity quantity_;
    const UUID uuid_;
    const TIF tif_;
    STATE state_;
    Quantity executed_quantity_;
    const TimePoint created_;
public:
    Order(const std::string_view &market_name,
          const SIDE &side,
          const Price &price,
          const Quantity &quantity,
          const UUID uuid = boost::uuids::random_generator()(),
          const TIF &tif = TIF::GTC,
          const STATE &state = STATE::INACTIVE,
          const Quantity &executed_quantity = 0,
          const TimePoint &created = Time::now()):
        market_name_{market_name},
        side_{side},
        price_{price},
        quantity_{quantity},
        uuid_{uuid},
        tif_{tif},
        state_{state},
        executed_quantity_{executed_quantity},
        created_{created} {}
    Order() = delete;
    Order(const Order &) = delete;
    Order& operator=(const Order&) = delete;
    ~Order() = default;

    std::string_view market_name() const;
    UUID uuid() const;
    void uuid(UUID);
    Quantity quantity() const;
    Price price() const;
    SIDE side() const;
    void execute(const Quantity &quantity);
    Quantity leftover() const;
    TIF tif() const;
    void state(STATE state);
    bool is_buy() const;
    /* Price/Time priority */
    bool operator>=(const Order &rhs) const;
    bool operator==(const Order &rhs) const;
};

using OrderPtr = std::unique_ptr<Order>;

using queue_allocator = boost::container::allocator<OrderPtr>;
using order_queue_type = boost::container::deque<OrderPtr, queue_allocator>;

class OrderQueue
    : public order_queue_type
{
public:
    OrderQueue() = default;
    OrderQueue(const OrderQueue &) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;
    template <typename order_type>
    OrderQueue(order_type order);
    template <typename order_type, typename... args>
    OrderQueue(order_type order, args... rest);
    bool remove(UUID uuid);
    Quantity accumulate() const;
};

class OrderBook
{
private:
    const std::string_view market_name_;
    struct Comp {
        enum compare_type { less, greater };
        explicit Comp(compare_type t) : type(t) {}
        template <typename lhs_type, typename rhs_type>
        bool operator()(const lhs_type &lhs, const rhs_type &rhs) const
        {
            return type == less ? lhs < rhs : lhs > rhs;
        }
        compare_type type;
    };
    using map_allocator = boost::container::adaptive_pool<std::pair<const Price, OrderQueue>>;
    using order_tree_type = boost::container::map<Price, OrderQueue, Comp, map_allocator>;
    order_tree_type buy_tree_;
    order_tree_type sell_tree_;
public:
    OrderBook(const std::string_view market_name):
        market_name_{market_name},
        buy_tree_{OrderBook::Comp{OrderBook::Comp::greater}},
        sell_tree_{OrderBook::Comp{OrderBook::Comp::less}}
    {
    }
    OrderBook(const OrderBook &) = delete;
    OrderBook& operator=(const OrderBook&) = delete;
    OrderBook() = delete;
    ~OrderBook() = default;
    bool cancel(const UUID uuid, SIDE side, Price price_node_hint);
    bool match(OrderPtr);
    Price best_buy() const;
    Price best_sell() const;
    Price quote() const;
    Price spread() const;
    std::string_view market_name() const;
    struct snapshot_point {
        Price price;
        Quantity cumulative_quantity;
        unsigned long size;
        SIDE side;
    };
    std::vector<snapshot_point> snapshot() const;
};

std::string_view Order::market_name() const
{
    return market_name_;
}
Quantity Order::quantity() const
{
    return quantity_;
}
UUID Order::uuid() const
{
    return uuid_;
}
Price Order::price() const
{
    return price_;
}
SIDE Order::side() const
{
    return side_;
}
void Order::execute(const Quantity &quantity)
{
    executed_quantity_ += quantity;
}
Quantity Order::leftover() const
{
    return quantity_ - executed_quantity_;
}
TIF Order::tif() const
{
    return tif_;
}
void Order::state(STATE state)
{
    state_ = state;
}
bool Order::is_buy() const
{
    return side_ == SIDE::BUY;
}

bool Order::operator>=(const Order &rhs) const
{
    return side_ == rhs.side_ && price_ == rhs.price_ &&
           created_ <= rhs.created_ && quantity_ >= rhs.quantity_;
}
bool Order::operator==(const Order &rhs) const
{
    return uuid_ == rhs.uuid_ && side_ == rhs.side_ && price_ == rhs.price_ &&
           quantity_ == rhs.quantity_;
}

template <typename order_type>
OrderQueue::OrderQueue(order_type order)
{
    emplace_back(std::move(order));
}

template <typename order_type, typename... args>
OrderQueue::OrderQueue(order_type order, args... rest)
{
    emplace_back(std::move(order));
    OrderQueue(rest...);
}

bool OrderQueue::remove(const UUID uuid)
{
    auto order =
    std::find_if(cbegin(), cend(), [&](const auto& it) {
        return it->uuid() == uuid;
    });
    if (order != cend()) {
        erase(order, cend());
        return true;
    }
    return false;
}

Quantity OrderQueue::accumulate() const
{
    return tbb::parallel_reduce(tbb::blocked_range<OrderQueue::const_iterator>(cbegin(),cend()), 0.0, [](const tbb::blocked_range<OrderQueue::const_iterator> r, Quantity total) {
        return std::accumulate(r.begin(), r.end(), total,
        [](const Quantity subtotal, const auto& order) {
            return subtotal + order->leftover();
        });
    }, std::plus<Quantity>());
}

bool OrderBook::cancel(const UUID uuid, const SIDE side, const Price price)
{
    auto &tree = side == SIDE::BUY ? buy_tree_ : sell_tree_;
    try {
        auto &&order_queue = tree.at(price);
        auto result = order_queue.remove(uuid);
        if (order_queue.empty()) /* Drop price node */
            tree.erase(price);
        return result;
    } catch (const std::out_of_range &) { /* No price point exist */
        return false;
    }
}

std::string_view OrderBook::market_name() const
{
    return market_name_;
}

bool OrderBook::match(OrderPtr src)
{
    auto &&src_tree = src->is_buy() ? buy_tree_ : sell_tree_;
    auto &&dist_tree = src->is_buy() ? sell_tree_ : buy_tree_;
    src->state(STATE::ACTIVE);

    auto should_exit_tree = false;
    for (auto node = dist_tree.begin(), end = dist_tree.end();
         !should_exit_tree && node != end;) {
        auto &&dist_queue = node->second;
        /* Buy cheap; sell expensive – conduct price improvement */
        if (src->is_buy() ? src->price() >= node->first
            : src->price() <= node->first) {
            for (auto exit_queue = false; !exit_queue && !dist_queue.empty();) {
                auto dist = dist_queue.front().get();
                auto leftover = dist->leftover() - src->leftover();

                /* Fulfilled source; partially or fulfilled dist */
                if (leftover >= 0) {
                    src->execute(src->leftover());
                    src->state(STATE::FULFILLED);
                    /* Exact match */
                    if (leftover == 0) {
                        dist->execute(dist->leftover());
                    }
                    /* Partial match */
                    else {
                        dist->execute(dist->leftover() - leftover);
                    }
                    /* Remove fulfilled order from queue */
                    if (dist->leftover() == 0) {
                        dist->state(STATE::FULFILLED);
                        dist_queue.pop_front();
                    }
                    /* Matching is complete */
                    exit_queue = true;
                    should_exit_tree = true;
                }
                /* Partially-filled source; fulfilled dist */
                else {
                    src->execute(dist->leftover());
                    dist->execute(dist->leftover());
                    dist->state(STATE::FULFILLED);
                    /* Remove fulfilled order from queue */
                    dist_queue.pop_front();
                    /* Try next order in the queue */
                }
            }
            /* Try next price node */
            if (dist_queue.empty()) {
                /* Purge the price point with empty queue */
                node = dist_tree.erase(node++);
            } else {
                ++node;
            }
        } else {
            should_exit_tree = true;
        }
    }
    /* Not enough resources to fulfill the order; push to source tree */
    if (src->leftover() > 0) {
        auto const& node = src_tree.find(src->price());
        if (node == src_tree.end()) { /* Create new price node */
            src_tree.emplace_hint(
                node,
                src->price(),
                std::move(src));
        } else { /* Insert in existing price node */
            node->second.emplace_back(std::move(src));
        }
        return false;
    }
    /* Order's been fulfilled */
    return true;
}

Price OrderBook::best_buy() const
{
    return !buy_tree_.empty()
           ? buy_tree_.begin()->first
           : !sell_tree_.empty() ? sell_tree_.begin()->first : 0;
}

Price OrderBook::best_sell() const
{
    return !sell_tree_.empty()
           ? sell_tree_.begin()->first
           : !buy_tree_.empty() ? buy_tree_.begin()->first : 0;
}

Price OrderBook::quote() const
{
    return (best_buy() + best_sell()) / 2;
}

Price OrderBook::spread() const
{
    auto buy = best_buy();
    auto sell = best_sell();
    return buy && sell ? (sell - buy) / sell : 0;
}

std::vector<OrderBook::snapshot_point> OrderBook::snapshot() const
{
    std::vector<snapshot_point> snapshot;
    auto traverse = [&](const auto &tree, const SIDE &side) {
        auto limit = 0;
        for (auto &&node : tree) {
            snapshot_point point;
            point.side = side;
            point.price = node.first;
            point.cumulative_quantity = node.second.accumulate();
            point.size = node.second.size();
            snapshot.emplace_back(point);
            limit++;
            if (limit >= 20)
                break;
        }
    };
    traverse(buy_tree_, SIDE::BUY);
    traverse(sell_tree_, SIDE::SELL);
    return snapshot;
}


} // namespace matching_engine

