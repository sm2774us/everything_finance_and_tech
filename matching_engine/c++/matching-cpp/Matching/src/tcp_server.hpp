#pragma once

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>
#include <nlohmann/json.hpp>
#include <order_router.hpp>

namespace matching_engine
{
namespace tcp
{
using boost::asio::ip::tcp;
namespace http = boost::beast::http;
using request_t = http::request<boost::beast::http::string_body>;
using response_t = http::response<boost::beast::http::string_body>;

class connection_handler
    : public std::enable_shared_from_this<connection_handler>
{
public:
    connection_handler(boost::asio::io_context &ioc,
                       const std::shared_ptr<router::dispatcher> dispatcher,
                       const std::shared_ptr<spdlog::logger>& console)
        : strand_{std::make_unique<boost::asio::io_context::strand>(ioc)},
          socket_{ioc}, work_{ioc}, dispatcher_{dispatcher}, console_{console} {}

    connection_handler(const connection_handler&) = delete;

    void dispatch()
    {
        auto self = shared_from_this();
        http::async_read(
            socket_, buffer_, request_,
        [this, self](boost::system::error_code ec, std::size_t) {
            if (ec == http::error::end_of_stream) {
                return;
            }
            if (ec) {
                console_->error("connection_handler::async_read: {}", ec.message());
                return;
            }
            //logger_->info("connection_handler::async_read: {}", request_.target().to_string());

            std::string_view target = request_.target();
            //boost::trim_if(target, [](auto ch) { return ch == '/'; });
            std::vector<std::string_view> params;
            //boost::split(params, target, [](auto ch) { return ch == '/'; },
            //    boost::token_compress_on);

            size_t first = 0;
            while (first < target.size()) {
                const auto second = target.find_first_of('/', first);
                if (first != second) params.emplace_back(target.substr(first, second-first));
                if (second == std::string_view::npos) break;
                first = second + 1;
            }

            //std::ostringstream ss;
            http::status status;
            if (params.size() < 4 or target.find(u8"favicon.ico") != std::string_view::npos) {
                console_->warn("connection_handler::async_read: Invalid request");
                //ss << nlohmann::json::parse("{\"target\":\""+target+"\",\"status\": \"FAILED\",\"origin\":\"" +
                //    boost::lexical_cast<std::string>(socket_.remote_endpoint()) + "\"}");
                status = http::status::bad_request;
            } else {
                SIDE side = params[0] == u8"BUY" ? SIDE::BUY : SIDE::SELL;
                std::string_view market = dispatcher_->registered_market_name(params[1]);
                Price price = std::stod(params[2].data());
                Quantity quantity = std::stod(params[3].data());
                dispatcher_->send(std::move(std::make_unique<Order>(market, side, price, quantity)));
                status = http::status::ok;
            }
            self->strand_->dispatch([self, status] { self->reply(status); });
        });
    }

    tcp::socket &socket()
    {
        return socket_;
    }

    void start()
    {
        strand_->dispatch([self = shared_from_this()] { self->dispatch(); });
    }

private:
    response_t static build_response(http::status status,
                                     http::request<http::string_body> &req)
    {
        response_t res{status, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        //res.body() = std::string(msg);
        res.prepare_payload();
        return res;
    }

    void reply(http::status status)
    {
        auto self = shared_from_this();
        auto response = std::make_shared<response_t>(
                            build_response(status, request_));
        http::async_write(
            socket_, *response,
        [this, self, response](boost::system::error_code ec, std::size_t) {
            if (ec) {
                console_->error("server::async_write: {}", ec.message());
                return;
            }
            if (response->need_eof())
                return;

            if (!ec)
                self->strand_->dispatch([self] { self->dispatch(); });
        });
    }

    tcp::socket socket_;
    std::unique_ptr<boost::asio::io_context::strand> strand_;
    boost::asio::io_context::work work_;
    boost::beast::flat_buffer buffer_;
    request_t request_;
    std::shared_ptr<router::dispatcher> dispatcher_;
    const std::shared_ptr<spdlog::logger>& console_;
};

class server
{
public:
    server(boost::asio::io_context &ioc,
           const std::shared_ptr<router::dispatcher> dispatcher,
           const std::shared_ptr<spdlog::logger> console,
           const short port = 8080,
           const uint64_t available_cores = std::max(1u, std::thread::hardware_concurrency() - 1))
        : ioc_{ioc}, acceptor_{ioc, tcp::endpoint(tcp::v6(), port)},
          dispatcher_{dispatcher}, console_{console}, pool_{available_cores}
    {
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        async_start_();
        /* Start event loop for all threads */
        for (auto core = available_cores; core > 0; --core) {
            boost::asio::post(pool_, [&] {ioc.run();});
        }
    }
    server(const server&) = delete;

    boost::system::error_code shutdown()
    {
        boost::system::error_code ec;
        acceptor_.close(ec);
        return ec;
    }

private:
    void async_start_()
    {
        const auto endpoint = boost::lexical_cast<std::string>(acceptor_.local_endpoint());
        acceptor_.listen(boost::asio::socket_base::max_listen_connections);
        if (acceptor_.is_open()) {
            console_->info("server::start: started on {}", endpoint);
        } else {
            console_->warn("server::start: failed to start on {}", endpoint);
        }
        async_accept_();
    }
    void async_accept_()
    {
        auto handler = std::make_shared<connection_handler>(ioc_, dispatcher_, console_);
        acceptor_.async_accept(
        handler->socket(), [this, handler](boost::system::error_code ec) {
            //logger_->info("server::async_accept: {}",
            //    boost::lexical_cast<std::string>(
            //      handler->socket().remote_endpoint()));
            if (ec) {
                console_->error("server::async_accept: {}", ec.message());
            } else {
                handler->dispatch();
            }
            async_accept_();
        });
    }

    tcp::acceptor acceptor_;
    boost::asio::io_context &ioc_;
    std::shared_ptr<router::dispatcher> dispatcher_;
    const std::shared_ptr<spdlog::logger> console_;
    boost::asio::thread_pool pool_;
};
} // namespace tcp
} // namespace matching_engine
