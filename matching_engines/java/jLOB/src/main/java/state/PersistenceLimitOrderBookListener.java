package state;

import cache.Cache;
import com.google.inject.Inject;
import db.persistence.CancellationsPersistence;
import db.persistence.MatchesPersistence;
import db.persistence.PlacementsPersistence;
import dto.Cancellation;
import dto.Match;
import dto.Placement;
import org.jooq.DSLContext;

import java.util.function.Supplier;


public class PersistenceLimitOrderBookListener implements LimitOrderBookListener {

    private final Cache cache;
    private final PlacementsPersistence placementsPersistence;
    private final CancellationsPersistence cancellationsPersistence;
    private final MatchesPersistence matchesPersistence;

    @Inject
    public PersistenceLimitOrderBookListener(Cache cache, Supplier<DSLContext> database) {
        this.cache = cache;
        this.placementsPersistence = new PlacementsPersistence(database);
        this.cancellationsPersistence = new CancellationsPersistence(database);
        this.matchesPersistence = new MatchesPersistence(database);
    }

    @Override
    public void onPlacement(Placement placement, LimitOrderBook limitOrderBook) {
        placementsPersistence.persistPlacement(placement);
        cache.cacheLimitOrderBook(limitOrderBook);
    }

    @Override
    public void onCancellation(Cancellation cancellation, LimitOrderBook limitOrderBook) {
        cancellationsPersistence.persistCancellation(cancellation);
        cache.cacheLimitOrderBook(limitOrderBook);
    }

    @Override
    public void onMatch(Match match) {
        matchesPersistence.persistMatch(match);
    }
}
