package connectivity.http;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.inject.Inject;
import dto.Cancellation;
import dto.Placement;
import dto.Side;
import spark.ExceptionHandler;
import spark.Route;
import state.LimitOrderBook;

import java.math.BigDecimal;
import java.time.Instant;
import java.util.Map;

public class LimitOrderBookApi {

    private LimitOrderBook limitOrderBook;
    private Gson gson;
    private JsonParser parser;


    @Inject
    public LimitOrderBookApi(LimitOrderBook limitOrderBook) {
        this.limitOrderBook = limitOrderBook;
        GsonBuilder gsonBuilder = new GsonBuilder();
        gsonBuilder.registerTypeAdapter(Placement.class, new PlacementDeserializer());
        gsonBuilder.registerTypeAdapter(Cancellation.class, new CancellationDeserializer());
        gsonBuilder.registerTypeAdapter(Instant.class, new InstantSerializer());
        this.gson = gsonBuilder.create();
        this.parser = new JsonParser();
    }

    public Route getLimitOrderBook = (request, response) -> gson.toJson(limitOrderBook);

    public Route placeOrder = (request, response) -> {
        Placement placement = gson.fromJson(request.body(), Placement.class);
        limitOrderBook.place(placement);
        return gson.toJson(placement);
    };

    public Route reduceOrder = (request, response) -> {
        Cancellation cancellation = gson.fromJson(request.body(), Cancellation.class);
        limitOrderBook.cancel(cancellation);
        return gson.toJson(cancellation);
    };

    public Route getVwap = (request, response) -> {
        JsonObject element = parser.parse(request.body()).getAsJsonObject();
        String action = element.get("action").getAsString();
        Side side = Side.fromString(action);
        long size = element.get("size").getAsLong();
        BigDecimal price = side == Side.BID ? limitOrderBook.getAveragePurchasePrice(size) :
                limitOrderBook.getAverageSalePrice(size);
        return gson.toJson(Map.of("price", price));
    };

    public ExceptionHandler<Exception> handleException = (exception, request, response) -> {
        response.status(422);
        response.body(gson.toJson(Map.of("error", exception.getMessage())));
    };
}
