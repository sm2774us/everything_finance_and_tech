package state;

import cache.Cache;
import com.google.inject.Inject;
import com.google.inject.Singleton;
import dto.Cancellation;
import dto.Placement;
import dto.Side;
import exceptions.JLOBException;
import it.unimi.dsi.fastutil.longs.Long2ObjectMap;
import it.unimi.dsi.fastutil.longs.Long2ObjectRBTreeMap;
import it.unimi.dsi.fastutil.longs.LongComparators;
import it.unimi.dsi.fastutil.objects.Object2ObjectOpenHashMap;
import org.apache.commons.lang3.builder.ToStringBuilder;

import java.io.Serializable;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Map;
import java.util.UUID;
import java.util.stream.Stream;

import static java.math.BigDecimal.valueOf;

/**
 * L3 Limit Order Book implementation.
 *
 * Bids and offers are kept sorted in reverse natural and natural orders respectively
 * by virtue of {@code Long2ObjectRBTreeMap<Limit>}s
 *
 * {@code Limit}s represent price levels in an order book and wrap around a collection of {@code Placement}s.
 *
 * Access to the object is thread-safe.
 */

@Singleton
public class LimitOrderBook implements Serializable {

    private final transient LimitOrderBookListener listener;
    private final Long2ObjectRBTreeMap<Limit> bids;
    private final Long2ObjectRBTreeMap<Limit> offers;
    private final transient Object2ObjectOpenHashMap<UUID, Placement> placements;

    @Inject
    public LimitOrderBook(LimitOrderBookListener listener, Cache cache) {
        this.listener = listener;
        if (cache.bookKeyExists()) {
            LimitOrderBook limitOrderBook = cache.getLimitOrderBook();
            this.bids = limitOrderBook.bids;
            this.offers = limitOrderBook.offers;
            this.placements = new Object2ObjectOpenHashMap<>();
            this.bids.long2ObjectEntrySet().stream()
                    .map(Map.Entry::getValue)
                    .forEach(limit -> limit
                        .forEach(placement -> this.placements.put(placement.getUuid(), placement)));

            this.offers.long2ObjectEntrySet().stream()
                    .map(Map.Entry::getValue)
                    .forEach(limit -> limit
                        .forEach(placement -> this.placements.put(placement.getUuid(), placement)));

        } else {
            this.bids = new Long2ObjectRBTreeMap<>(LongComparators.OPPOSITE_COMPARATOR);
            this.offers = new Long2ObjectRBTreeMap<>(LongComparators.NATURAL_COMPARATOR);
            this.placements = new Object2ObjectOpenHashMap<>();
        }
    }

    private LimitOrderBook(LimitOrderBookListener listener) {
        this.listener = listener;
        this.bids = new Long2ObjectRBTreeMap<>(LongComparators.OPPOSITE_COMPARATOR);
        this.offers = new Long2ObjectRBTreeMap<>(LongComparators.NATURAL_COMPARATOR);
        this.placements = new Object2ObjectOpenHashMap<>();
    }

    public static LimitOrderBook empty() {
        return new LimitOrderBook(new DummyLimitOrderBookListener());
    }

    public synchronized void place(Placement placement) {
        if (placements.containsKey(placement.getUuid()))
            return;
        if (placement.getSide() == Side.BID)
            bid(placement);
        else
            offer(placement);

        listener.onPlacement(placement, this);
    }

    private void bid(Placement placement) {
        Placement takerPlacement = placement.copy();
        Limit limit  = getBestLimit(offers);
        while (takerPlacement.getSize() > 0 && limit != null && limit.getPrice() <= takerPlacement.getPrice()) {
            takerPlacement = limit.match(takerPlacement, placements::remove, listener::onMatch);
            if (limit.isEmpty())
                offers.remove(limit.getPrice());
            limit = getBestLimit(offers);
        }
        if (takerPlacement.getSize() > 0) {
            placements.put(placement.getUuid(), place(bids, takerPlacement));
        }
    }

    private void offer(Placement placement) {
        Placement takerPlacement = placement.copy();
        Limit limit = getBestLimit(bids);
        while (takerPlacement.getSize() > 0 && limit != null && limit.getPrice() >= takerPlacement.getPrice()) {
            takerPlacement = limit.match(takerPlacement, placements::remove, listener::onMatch);
            if (limit.isEmpty())
                bids.remove(limit.getPrice());
            limit = getBestLimit(bids);
        }
        if (takerPlacement.getSize() > 0) {
            placements.put(takerPlacement.getUuid(), place(offers, takerPlacement));
        }
    }

    private Placement place(Long2ObjectRBTreeMap<Limit> levels, Placement placement) {
        Limit level = levels.get(placement.getPrice());
        if (level == null) {
            level = new Limit(placement.getSide(), placement.getPrice());
            levels.put(placement.getPrice(), level);
        }
        return level.place(placement);
    }

    public synchronized boolean cancel(Cancellation cancellation) {
        Placement placement = placements.get(cancellation.getPlacementUuid());
        if (placement == null || cancellation.getSize() > placement.getSize())
            throw new JLOBException("Placement does not exist or cancellation size is inappropriate");

        boolean isFullCancellation;
        if (cancellation.getSize() == placement.getSize()) {
            remove(placement);
            isFullCancellation = true;
        } else {
            placement.reduce(cancellation.getSize());
            isFullCancellation = false;
        }
        listener.onCancellation(cancellation, this);
        return isFullCancellation;
    }

    private void remove(Placement placement) {
        Side side = placement.getSide();
        long price = placement.getPrice();
        Limit limit;

        if (side == Side.BID)
            limit = bids.get(price);
        else
            limit = offers.get(price);

        limit.remove(placement);
        if (limit.isEmpty())
            remove(limit);

        placements.remove(placement.getUuid());
    }

    private void remove(Limit limit) {
        if (limit.getSide() == Side.BID)
            bids.remove(limit.getPrice());
        else
            offers.remove(limit.getPrice());
    }

    private Limit getBestLimit(Long2ObjectRBTreeMap<Limit> levels) {
        if (levels.isEmpty())
            return null;
        return levels.get(levels.firstLongKey());
    }

    public long getMidPrice(){
        return (bids.firstLongKey() + offers.firstLongKey()) / 2;
    }

    public synchronized Stream<Long2ObjectMap.Entry<Limit>> streamBids() {
        return bids.long2ObjectEntrySet().stream();
    }

    public synchronized Stream<Long2ObjectMap.Entry<Limit>> streamOffers() {
        return offers.long2ObjectEntrySet().stream();
    }

    public long getBestBid(){
        return bids.firstLongKey();
    }

    public long getBestOffer(){
        return offers.firstLongKey();
    }

    public long getBestBidAmount(){
        return bids.get(bids.firstLongKey()).getVolume();
    }

    public long getBestOfferAmount(){
        return offers.get(offers.firstLongKey()).getVolume();
    }

    private BigDecimal getAveragePrice(long size, Long2ObjectRBTreeMap<Limit> levels) {
        long psizesum = 0L, sizesum = 0L;
        for (Limit limit : levels.values()) {
            long unfilled_size = size - sizesum;
            long price = limit.getPrice();
            long volume = limit.getVolume();
            long s = Math.min(unfilled_size, volume);
            sizesum += s;
            psizesum += s * price;
            if (sizesum >= size)
                return valueOf(sizesum).equals(BigDecimal.ZERO) ? BigDecimal.ZERO :
                        valueOf(psizesum).divide(valueOf(size), 2, RoundingMode.HALF_UP);
        }
        return BigDecimal.ZERO;
    }

    public BigDecimal getAverageSalePrice(long size){
        return getAveragePrice(size, bids);
    }

    public BigDecimal getAveragePurchasePrice(long size){
        return getAveragePrice(size, offers);
    }

    @Override
    public String toString() {
        return new ToStringBuilder(this)
                .append("bids", bids)
                .append("offers", offers)
                .toString();
    }
}