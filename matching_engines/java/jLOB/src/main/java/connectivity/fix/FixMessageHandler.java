package connectivity.fix;

import dto.Cancellation;
import dto.Placement;
import dto.Side;
import quickfix.FieldNotFound;
import quickfix.Message;
import quickfix.field.*;
import quickfix.fix42.ExecutionReport;
import quickfix.fix42.MarketDataSnapshotFullRefresh;
import state.LimitOrderBook;

import java.math.BigDecimal;
import java.util.Map;

import static dto.Placement.placement;
import static java.util.Arrays.stream;
import static java.util.UUID.fromString;
import static java.util.UUID.randomUUID;
import static java.util.function.Function.identity;
import static java.util.stream.Collectors.toMap;

public enum FixMessageHandler {

    MARKET_DATA_REQUEST {
        @Override
        String getRequestMsgType() {
            return MsgType.MARKET_DATA_REQUEST;
        }

        @Override
        Message handle(Message message, LimitOrderBook limitOrderBook) {
            MarketDataSnapshotFullRefresh msg = new MarketDataSnapshotFullRefresh();
            limitOrderBook.streamBids()
                    .forEachOrdered(limitEntry -> {
                        MarketDataSnapshotFullRefresh.NoMDEntries group = new MarketDataSnapshotFullRefresh.NoMDEntries();
                        group.set(new MDEntryType('0'));
                        group.set(new MDEntryPx(limitEntry.getLongKey()));
                        group.set(new MDEntrySize(limitEntry.getValue().getVolume()));
                        msg.addGroup(group);
                    });

            limitOrderBook.streamOffers()
                    .forEachOrdered(limitEntry -> {
                        MarketDataSnapshotFullRefresh.NoMDEntries group = new MarketDataSnapshotFullRefresh.NoMDEntries();
                        group.set(new MDEntryType('1'));
                        group.set(new MDEntryPx(limitEntry.getLongKey()));
                        group.set(new MDEntrySize(limitEntry.getValue().getVolume()));
                        msg.addGroup(group);
                    });

            return msg;
        }
    },

    NEW_SINGLE_ORDER {
        @Override
        String getRequestMsgType() {
            return MsgType.ORDER_SINGLE;
        }

        @Override
        Message handle(Message message, LimitOrderBook limitOrderBook) throws FieldNotFound {
            char msgSide = message.getChar(quickfix.field.Side.FIELD);
            Side side = msgSide == '1' ? Side.BID : Side.OFFER;
            long price = (long) message.getDouble(Price.FIELD);
            long amount = (long) message.getDouble(OrderQty.FIELD);
            Placement placement = placement().withSide(side).withPrice(price).withSize(amount).build();
            String placementId = "" + placement.getUuid();
            BigDecimal averageSalePrice = limitOrderBook.getAverageSalePrice(amount);
            limitOrderBook.place(placement);
            char ordStatus = averageSalePrice.equals(BigDecimal.ZERO) ? OrdStatus.NEW : OrdStatus.FILLED;
            double leavesQty = averageSalePrice.equals(BigDecimal.ZERO) ? amount : 0.;

            final ExecutionReport executionReport = new ExecutionReport();
            executionReport.set(new OrderID(placementId));
            executionReport.set(new ExecID(randomUUID().toString()));
            executionReport.set(new ExecTransType(ExecTransType.NEW));
            executionReport.set(new ExecType(ordStatus));
            executionReport.set(new OrdStatus(ordStatus));
            executionReport.set(new quickfix.field.Side(msgSide));
            executionReport.set(new LeavesQty(leavesQty));
            executionReport.set(new CumQty(amount - leavesQty));
            executionReport.set(new AvgPx(averageSalePrice.doubleValue()));
            return executionReport;
        }
    },

    CANCEL_REPLACE_ORDER {
        @Override
        String getRequestMsgType() {
            return MsgType.ORDER_CANCEL_REPLACE_REQUEST;
        }

        @Override
        Message handle(Message message, LimitOrderBook limitOrderBook) throws FieldNotFound {
            String orderId = message.getString(OrderID.FIELD);
            long cancellationAmount = (long) message.getDouble(OrderQty.FIELD);
            Cancellation cancellation = new Cancellation(fromString(orderId), cancellationAmount);
            boolean isFullCancellation = limitOrderBook.cancel(cancellation);
            final char execType = isFullCancellation ? ExecType.CANCELED : ExecType.REPLACED;
            ExecutionReport executionReport = new ExecutionReport();
            executionReport.set(new ExecType(execType));
            return executionReport;
        }
    };

    public static final Map<String, FixMessageHandler> handlerMap = stream(values()).collect(
            toMap(FixMessageHandler::getRequestMsgType, identity())
    );

    public static FixMessageHandler lookUp(String msgType) {
        return handlerMap.get(msgType);
    }

    abstract String getRequestMsgType();
    abstract Message handle(Message message,  LimitOrderBook limitOrderBook) throws FieldNotFound;

}
