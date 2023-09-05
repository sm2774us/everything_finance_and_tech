package state;

import dto.Match;
import dto.Placement;
import dto.Side;

import java.io.Serializable;
import java.util.*;
import java.util.function.Consumer;

public class Limit implements Serializable, Iterable<Placement> {

    private final Side side;
    private final long price;
    private final List<Placement> placements;

    public Limit(Side side, long price) {
        this.side = side;
        this.price = price;
        this.placements = new ArrayList<>();
    }

    public Side getSide() {
        return side;
    }

    public long getPrice() {
        return price;
    }

    public Placement place(Placement placement){
        placements.add(placement);
        return placement;
    }

    public void remove(Placement placement){
        placements.remove(placement);
    }

    public Placement match(Placement takerPlacement, Consumer<UUID> placementRemoveCallback,
                           Consumer<Match> matchCallback) {
        while (takerPlacement.getSize() > 0 && !placements.isEmpty()) {
            Placement makerPlacement = placements.get(0);
            UUID makerPlacementId = makerPlacement.getUuid();
            long makerPlacementSize = makerPlacement.getSize();
            long takerPlacementUnfilledSize = takerPlacement.getSize();
            if (makerPlacementSize > takerPlacementUnfilledSize) {
                makerPlacement.reduce(takerPlacementUnfilledSize);
                takerPlacement.reduce(takerPlacementUnfilledSize);
                matchCallback.accept(new Match(makerPlacement.getUuid(), takerPlacement.getUuid(), takerPlacementUnfilledSize));
            } else {
                placements.remove(0);
                takerPlacement.reduce(makerPlacementSize);
                placementRemoveCallback.accept(makerPlacementId);
                matchCallback.accept(new Match(makerPlacement.getUuid(), takerPlacement.getUuid(), makerPlacementSize));
            }
        }
        return takerPlacement;
    }

    public long getVolume(){
        return placements.stream().map(Placement::getSize).reduce(0L, Long::sum);
    }

    public int getPlacementCount(){
        return placements.size();
    }

    public boolean isEmpty(){
        return placements.isEmpty();
    }

    @Override
    public String toString() {
        return "Limit{" +
                "side=" + side +
                ", price=" + price +
                ", placements=" + placements +
                '}';
    }

    @Override
    public Iterator<Placement> iterator() {
        return placements.iterator();
    }

    @Override
    public void forEach(Consumer<? super Placement> action) {
        placements.forEach(action);
    }

    @Override
    public Spliterator<Placement> spliterator() {
        return placements.spliterator();
    }
}