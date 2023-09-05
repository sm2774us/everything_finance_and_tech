package state;

import dto.Placement;
import dto.Side;
import org.junit.jupiter.api.Test;

import static dto.Placement.placement;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;


public class LimitTest {

    @Test
    public void testLimitCreation() {
        Limit limit = new Limit(Side.BID, 100L);
        assertTrue(limit.isEmpty());
        assertEquals(Side.BID, limit.getSide());
        assertEquals(100L, limit.getPrice());
    }

    @Test
    public void testLimitPlace() {
        Limit limit = new Limit(Side.BID, 100L);

        Placement placement = placement().withSide(Side.BID).withPrice(limit.getPrice()).withSize(10).build();
        Placement placement1 = placement().withSide(Side.BID).withPrice(limit.getPrice()).withSize(10).build();
        Placement placement2 = placement().withSide(Side.BID).withPrice(limit.getPrice()).withSize(10).build();

        assertEquals(placement.getSide(), limit.getSide());
        assertEquals(placement.getPrice(), limit.getPrice());

        limit.place(placement);
        limit.place(placement1);
        limit.place(placement2);

        assertEquals(3, limit.getPlacementCount());
        assertEquals(30L, limit.getVolume());
    }

    @Test
    public void testLimitRemove() {
        Limit limit = new Limit(Side.BID, 100L);

        Placement placement = placement().withSide(Side.BID).withPrice(limit.getPrice()).withSize(10).build();
        Placement placement1 = placement().withSide(Side.BID).withPrice(limit.getPrice()).withSize(10).build();
        Placement placement2 = placement().withSide(Side.BID).withPrice(limit.getPrice()).withSize(10).build();

        limit.place(placement);limit.place(placement1);limit.place(placement2);
        limit.remove(placement);limit.remove(placement1);limit.remove(placement2);

        assertTrue(limit.isEmpty());
    }
}
