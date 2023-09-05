package state;

import dto.Cancellation;
import dto.Match;
import dto.Placement;

public interface LimitOrderBookListener {
    void onPlacement(Placement placement, LimitOrderBook limitOrderBook);
    void onCancellation(Cancellation cancellation, LimitOrderBook limitOrderBook);
    void onMatch(Match match);
}
