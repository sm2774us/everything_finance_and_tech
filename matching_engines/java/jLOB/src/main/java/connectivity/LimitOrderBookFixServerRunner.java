package connectivity;

import com.google.inject.Inject;
import connectivity.fix.FixServer;
import state.LimitOrderBook;


public class LimitOrderBookFixServerRunner implements ServerRunner {

    private final Runnable serverRunnable;

    @Inject
    public LimitOrderBookFixServerRunner(LimitOrderBook limitOrderBook) {
        this.serverRunnable = new FixServer(limitOrderBook).serverRunnable;
    }

    @Override
    public void run() {
        new Thread(serverRunnable).start();
    }
}
