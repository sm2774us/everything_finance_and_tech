package dto;

import exceptions.JLOBException;
import org.junit.jupiter.api.Test;

import static dto.Placement.placement;
import static org.junit.jupiter.api.Assertions.*;

public class PlacementTest {

    private final Placement placement = placement()
            .withSide(Side.BID)
            .withPrice(100)
            .withSize(10)
            .build();

    @Test
    public void testPlacementCreation() {
        assertEquals(100L, placement.getPrice());
        assertEquals(10L, placement.getSize());
        assertEquals(Side.BID, placement.getSide());
    }

    @Test
    public void testPartialReduce() {
        placement.reduce(5L);
        assertEquals(5L, placement.getSize());
    }

    @Test
    public void testFullReduce() {
        placement.reduce(10L);
        assertEquals(0L, placement.getSize());
    }

    @Test
    public void testZeroPlacementPrice() {
        Exception exception = assertThrows(
                IllegalArgumentException.class,
                () -> placement().withSide(Side.BID).withPrice(0).withSize(100).build()
        );

        assertEquals("Invalid placement price", exception.getMessage());
    }

    @Test
    public void testNegativePlacementPrice() {
        Exception exception = assertThrows(
                IllegalArgumentException.class,
                () -> placement().withSide(Side.BID).withPrice(-100).withSize(100).build()
        );

        assertEquals("Invalid placement price", exception.getMessage());
    }

    @Test
    public void testZeroPlacementSize() {
        Exception exception = assertThrows(
                IllegalArgumentException.class,
                () -> placement().withSide(Side.BID).withPrice(100).withSize(0).build()
        );

        assertEquals("Invalid placement size", exception.getMessage());
    }

    @Test
    public void testNegativePlacementSize() {
        Exception exception = assertThrows(
                IllegalArgumentException.class,
                () -> placement().withSide(Side.BID).withPrice(100).withSize(-100).build()
        );

        assertEquals("Invalid placement size", exception.getMessage());
    }
}