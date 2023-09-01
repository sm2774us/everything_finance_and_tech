package state;

import cache.Cache;
import dto.Cancellation;
import dto.Match;
import static dto.Match.match;
import dto.Side;
import exceptions.JLOBException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import java.util.UUID;

import static dto.Placement.placement;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.contains;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.*;

public class LimitOrderBookTest {

    private LimitOrderBook book;
    private LimitOrderBookListener listener;

    @BeforeEach
    protected void setUp() {
        this.listener = mock(LimitOrderBookListener.class);
        Cache cache = mock(Cache.class);
        book = new LimitOrderBook(listener, cache);

        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1300)
                .withSize(245)
                .build());

        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1200)
                .withSize(25)
                .build());

        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1100)
                .withSize(125)
                .build());

        book.place(placement()
                .withSide(Side.BID)
                .withPrice(1000)
                .withSize(100)
                .build());

        book.place(placement()
                .withSide(Side.BID)
                .withPrice(900)
                .withSize(75)
                .build());

        book.place(placement()
                .withSide(Side.BID)
                .withPrice(800)
                .withSize(125)
                .build());
    }

    @Test
    public void testBuy() {
        book.place(placement()
                .withSide(Side.BID)
                .withPrice(1000)
                .withSize(100)
                .build());
        assertEquals(1000, book.getBestBid());
        assertEquals(1100, book.getBestOffer());
        assertEquals(200, book.getBestBidAmount());
        assertEquals(125, book.getBestOfferAmount());
        assertEquals(1050, book.getMidPrice());
    }

    @Test
    public void testBuyCross() {
        book.place(placement()
                .withSide(Side.BID)
                .withPrice(1100)
                .withSize(100)
                .build());
        assertEquals(1000, book.getBestBid());
        assertEquals(1100, book.getBestOffer());
        assertEquals(100, book.getBestBidAmount());
        assertEquals(25, book.getBestOfferAmount());
        assertEquals(1050, book.getMidPrice());
    }

    @Test
    public void testBestBuyFill() {
        book.place(placement()
                .withSide(Side.BID)
                .withPrice(1100)
                .withSize(125)
                .build());
        assertEquals(1000, book.getBestBid());
        assertEquals(1200, book.getBestOffer());
        assertEquals(25, book.getBestOfferAmount());
        assertEquals(100, book.getBestBidAmount());
        assertEquals(1100, book.getMidPrice());
    }

    @Test
    public void testBestBuyOverfill() {
        book.place(placement()
                .withSide(Side.BID)
                .withPrice(1100)
                .withSize(225)
                .build());
        assertEquals(1100, book.getBestBid());
        assertEquals(1200, book.getBestOffer());
        assertEquals(100, book.getBestBidAmount());
        assertEquals(25, book.getBestOfferAmount());
        assertEquals(1150, book.getMidPrice());
    }

    @Test
    public void testSell() {
        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1100)
                .withSize(100)
                .build());
        assertEquals(1000, book.getBestBid());
        assertEquals(1100, book.getBestOffer());
        assertEquals(100, book.getBestBidAmount());
        assertEquals(225, book.getBestOfferAmount());
        assertEquals(1050, book.getMidPrice());
    }

    @Test
    public void testBestSellCross() {
        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1000)
                .withSize(50)
                .build());
        assertEquals(1000, book.getBestBid());
        assertEquals(1100, book.getBestOffer());
        assertEquals(50, book.getBestBidAmount());
        assertEquals(125, book.getBestOfferAmount());
        assertEquals(1050, book.getMidPrice());
    }

    @Test
    public void testBestSellFill() {
        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1000)
                .withSize(100)
                .build());
        assertEquals(900, book.getBestBid());
        assertEquals(1100, book.getBestOffer());
        assertEquals(75, book.getBestBidAmount());
        assertEquals(125, book.getBestOfferAmount());
        assertEquals(1000, book.getMidPrice());
    }

    @Test
    public void testBestSellOverFill() {
        book.place(placement()
                .withSide(Side.OFFER)
                .withPrice(1000)
                .withSize(400)
                .build());
        assertEquals(900, book.getBestBid());
        assertEquals(1000, book.getBestOffer());
        assertEquals(75, book.getBestBidAmount());
        assertEquals(300, book.getBestOfferAmount());
        assertEquals(950, book.getMidPrice());
    }

    @Test
    public void testCancellationAmountGreaterThanPlacement() {
        final var placement = placement()
                .withSide(Side.BID)
                .withPrice(100)
                .withSize(100)
                .build();
        Cancellation cancellation = new Cancellation(placement.getUuid(), 200);
        book.place(placement);
        assertThrows(
                JLOBException.class,
                () -> book.cancel(cancellation),
                "Placement does not exist or cancellation size is inappropriate"
        );
    }

    @Test
    public void testCancellationOfNonexistentPlacement() {
        final var placement = placement()
                .withSide(Side.BID)
                .withPrice(100)
                .withSize(100)
                .build();
        Cancellation cancellation = new Cancellation(UUID.randomUUID(), 200);
        book.place(placement);
        assertThrows(
                JLOBException.class,
                () -> book.cancel(cancellation),
                "Placement does not exist or cancellation size is inappropriate"
        );
    }

    @Test
    public void testAveragePurchaseTop() {
        assertEquals(1100.0, book.getAveragePurchasePrice(25).doubleValue());
    }

    @Test
    public void testAveragePurchaseMultipleLevels(){
        double result = (1100. * 125. + 1200. * 25.) / 150.;
        assertEquals(result, book.getAveragePurchasePrice(150).doubleValue(), 0.01);
    }

    @Test
    public void testAveragePurchaseOverflow(){
        assertEquals(0., book.getAveragePurchasePrice(1000).doubleValue());
    }

    @Test
    public void testAveragePurchaseZero(){
        assertEquals(0., book.getAveragePurchasePrice(0).doubleValue());
    }

    @Test
    public void testAverageSaleTop(){
        assertEquals(1000., book.getAverageSalePrice(50).doubleValue());
    }

    @Test
    public void testAverageSaleMultipleLevels(){
        double result = (1000. * 100. + 900. * 75.) / 175.;
        assertEquals(result, book.getAverageSalePrice(175).doubleValue(), 0.01);
    }

    @Test
    public void testAverageSalePurchaseOverflow(){
        assertEquals(0., book.getAverageSalePrice(1000).doubleValue());
    }

    @Test
    public void testAverageSaleZero(){
        assertEquals(0., book.getAverageSalePrice(0).doubleValue());
    }

    @Test
    public void shouldExecuteOnFifoBasis() {
        var placement1 = placement()
                .withSide(Side.BID)
                .withPrice(1050)
                .withSize(100)
                .build();

        var placement2 = placement()
                .withSide(Side.BID)
                .withPrice(1050)
                .withSize(200)
                .build();

        var placement3 = placement()
                .withSide(Side.BID)
                .withPrice(1050)
                .withSize(50)
                .build();

        var placement4 = placement()
                .withSide(Side.OFFER)
                .withPrice(1050)
                .withSize(1000)
                .build();

        book.place(placement1);
        book.place(placement2);
        book.place(placement3);
        book.place(placement4);

        ArgumentCaptor<Match> matchArgumentCaptor = ArgumentCaptor.forClass(Match.class);
        verify(listener, times(3)).onMatch(matchArgumentCaptor.capture());
        var matches = matchArgumentCaptor.getAllValues();

        var match1 = match()
                .withTimestamp(matches.get(0).getTimestamp())
                .withMakerPlacementUuid(placement1.getUuid())
                .withTakerPlacementUuid(placement4.getUuid())
                .withSize(placement1.getSize())
                .build();

        var match2 = match()
                .withTimestamp(matches.get(1).getTimestamp())
                .withMakerPlacementUuid(placement2.getUuid())
                .withTakerPlacementUuid(placement4.getUuid())
                .withSize(placement2.getSize())
                .build();

        var match3 = match()
                .withTimestamp(matches.get(2).getTimestamp())
                .withMakerPlacementUuid(placement3.getUuid())
                .withTakerPlacementUuid(placement4.getUuid())
                .withSize(placement3.getSize())
                .build();

        assertThat(matches, contains(match1, match2, match3));
    }
}