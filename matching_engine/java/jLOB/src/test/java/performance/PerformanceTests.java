package performance;

import dto.Cancellation;
import dto.Placement;
import dto.Side;
import org.openjdk.jmh.annotations.*;
import state.LimitOrderBook;

import java.util.concurrent.TimeUnit;

import static dto.Placement.placement;

public class PerformanceTests {

    @State(Scope.Thread)
    public static class MyState {
        LimitOrderBook book;
        Placement bidPlacement = placement().withSide(Side.BID).withPrice(100).withSize(100).build();
        Placement offerPlacement = placement().withSide(Side.OFFER).withPrice(100).withSize(100).build();
        Cancellation cancellation = new Cancellation(bidPlacement.getUuid(), 100);

        @Setup(Level.Invocation)
        public void doSetup() {
            book = LimitOrderBook.empty();
            book.place(bidPlacement);
        }

    }

    @Benchmark @BenchmarkMode(Mode.AverageTime) @OutputTimeUnit(TimeUnit.NANOSECONDS)
    public void testPlacement(MyState state) {
        /**
         * Benchmark                       Mode  Cnt    Score   Error  Units
         * PerformanceTests.testPlacement  avgt   25  127.061 ± 3.260  ns/op
         */
        state.book.place(state.bidPlacement);
    }

    @Benchmark @BenchmarkMode(Mode.AverageTime) @OutputTimeUnit(TimeUnit.NANOSECONDS)
    public void testCancellation(MyState state) {
        /**
         * Benchmark                          Mode  Cnt   Score   Error  Units
         * PerformanceTests.testCancellation  avgt   25  62.609 ± 1.535  ns/op
         */
        state.book.cancel(state.cancellation);
    }

    @Benchmark @BenchmarkMode(Mode.AverageTime) @OutputTimeUnit(TimeUnit.NANOSECONDS)
    public void testMatch(MyState state) {
        /**
         * Benchmark                   Mode  Cnt    Score   Error  Units
         * PerformanceTests.testMatch  avgt   25  122.326 ± 2.526  ns/op
         */
        state.book.place(state.offerPlacement);
    }
}
