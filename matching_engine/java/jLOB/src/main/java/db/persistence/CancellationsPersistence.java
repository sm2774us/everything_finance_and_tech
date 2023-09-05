package db.persistence;

import db.jooq.tables.records.CancellationsRecord;
import dto.Cancellation;
import org.jooq.DSLContext;

import java.util.function.Supplier;

public class CancellationsPersistence {

    private final Supplier<DSLContext> database;

    public CancellationsPersistence(Supplier<DSLContext> database) {
        this.database = database;
    }

    public void persistCancellation(Cancellation cancellation) {
        database.get().executeInsert(toRecord(cancellation));
    }

    public static CancellationsRecord toRecord(Cancellation cancellation) {
        return new CancellationsRecord(
                cancellation.getPlacementUuid(),
                cancellation.getTimestamp(),
                cancellation.getSize()
        );
    }
}
