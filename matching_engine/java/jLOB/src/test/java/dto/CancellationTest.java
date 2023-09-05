package dto;

import org.junit.jupiter.api.Test;

import static dto.Placement.placement;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class CancellationTest {

    private final Placement placement = placement()
            .withSide(Side.BID)
            .withPrice(100)
            .withSize(10)
            .build();

    @Test
    public void testCancellationCreation() {
        Cancellation cancellation = new Cancellation(placement.getUuid(),5);
        assertEquals(100L, placement.getPrice());
        assertEquals(10L, placement.getSize());
        assertEquals(5L, cancellation.getSize());
        assertEquals(cancellation.getPlacementUuid(), placement.getUuid());
    }

    @Test
    public void testZeroCancellationSize() {
        Exception exception = assertThrows(
                IllegalArgumentException.class,
                () -> new Cancellation(placement.getUuid(),0)
        );

        assertEquals("Invalid cancellation size", exception.getMessage());
    }

    @Test
    public void testNegativeCancellationSize() {
        Exception exception = assertThrows(
                IllegalArgumentException.class,
                () -> new Cancellation(placement.getUuid(),-1)
        );

        assertEquals("Invalid cancellation size", exception.getMessage());
    }
}
