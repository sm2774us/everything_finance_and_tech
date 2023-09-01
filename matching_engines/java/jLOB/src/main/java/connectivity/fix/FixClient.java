package connectivity.fix;

import quickfix.*;
import quickfix.field.*;
import quickfix.fix42.NewOrderSingle;

import java.time.LocalDateTime;
import java.util.UUID;

public class FixClient implements Application {

    /**
     * Example FIX client application
     */

    private volatile SessionID sessionID;

    public static Runnable clientRunnable = () -> {
        try {
            start();
        } catch (InterruptedException | ConfigError | SessionNotFound e) {
            e.printStackTrace();
        }
    };

    @Override
    public void onCreate(SessionID sessionId) {
        System.out.println("On create");
    }

    @Override
    public void onLogon(SessionID sessionId) {
        this.sessionID = sessionId;
        System.out.println("On logon");
    }

    @Override
    public void onLogout(SessionID sessionId) {
        System.out.println("On logout");
    }

    @Override
    public void toAdmin(Message message, SessionID sessionId) {
        System.out.println("To admin " + message.toXML());
    }

    @Override
    public void fromAdmin(Message message, SessionID sessionId) {
        System.out.println("From admin " + message.toXML());
    }

    @Override
    public void toApp(Message message, SessionID sessionId) {
        System.out.println("To app " + message.toXML());
    }

    @Override
    public void fromApp(Message message, SessionID sessionId) {
        System.out.println("From app " + message.toXML());
    }

    public static void start() throws ConfigError, SessionNotFound, InterruptedException {
        SessionSettings settings = new SessionSettings("fix.client.cfg");

        FixClient application = new FixClient();
        MessageStoreFactory messageStoreFactory = new MemoryStoreFactory();
        LogFactory logFactory = new ScreenLogFactory( false, false, false);
        MessageFactory messageFactory = new DefaultMessageFactory();

        Initiator initiator = new SocketInitiator(
                application,
                messageStoreFactory,
                settings,
                logFactory,
                messageFactory
        );
        initiator.start();

        while (application.sessionID == null) {
            Thread.sleep(100);
        }

        Message orderMsg = new NewOrderSingle(
                new ClOrdID(UUID.randomUUID().toString()),
                new HandlInst(HandlInst.MANUAL_ORDER_BEST_EXECUTION),
                new Symbol("JLOB"),
                new Side(Side.BUY),
                new TransactTime(LocalDateTime.now()),
                new OrdType(OrdType.LIMIT)
        );

        orderMsg.setDouble(Price.FIELD, 100);
        orderMsg.setDouble(OrderQty.FIELD, 100);
        Session.sendToTarget(orderMsg, application.sessionID);


        Message mktDataReqMsg = new Message();
        mktDataReqMsg.getHeader().setString(MsgType.FIELD, MsgType.MARKET_DATA_REQUEST);
        mktDataReqMsg.setString(Symbol.FIELD, "JLOB");
        mktDataReqMsg.setString(MDReqID.FIELD, UUID.randomUUID().toString());
        mktDataReqMsg.setChar(SubscriptionRequestType.FIELD, '0');
        mktDataReqMsg.setInt(MarketDepth.FIELD, 0);


        mktDataReqMsg.setChar(SubscriptionRequestType.FIELD, '0');
        mktDataReqMsg.setInt(MarketDepth.FIELD, '0');

        Session.sendToTarget(mktDataReqMsg, application.sessionID);
    }
}
