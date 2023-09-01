package connectivity;

import com.google.inject.Inject;
import connectivity.http.LimitOrderBookApi;
import state.LimitOrderBook;

import static spark.Spark.*;


public class LimitOrderBookHttpServerRunner implements ServerRunner {

    private static final String ORIGIN = "";
    private final LimitOrderBookApi limitOrderBookApi;

    @Inject
    public LimitOrderBookHttpServerRunner(LimitOrderBook limitOrderBook) {
        this.limitOrderBookApi = new LimitOrderBookApi(limitOrderBook);
    }

    @Override
    public void run() {
        before((request, response) -> response.header("Access-Control-Allow-Origin", "*"));
        options("/*", (request, response) -> {
            String accessControlRequestHeaders = request.headers("Access-Control-Request-Headers");
            if (accessControlRequestHeaders != null) {
                response.header("Access-Control-Allow-Headers", accessControlRequestHeaders);
            }

            String accessControlRequestMethod = request.headers("Access-Control-Request-Method");
            if (accessControlRequestMethod != null) {
                response.header("Access-Control-Allow-Methods", accessControlRequestMethod);
            }

            return "OK";
        });
        path("/book", () -> {
            get(ORIGIN, limitOrderBookApi.getLimitOrderBook);
            post(ORIGIN, limitOrderBookApi.placeOrder);
            put(ORIGIN, limitOrderBookApi.reduceOrder);
        });
        post("vwap", limitOrderBookApi.getVwap);
        exception(Exception.class, limitOrderBookApi.handleException);
    }
}
