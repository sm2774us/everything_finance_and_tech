package db.persistence;

import db.jooq.tables.records.PlacementsRecord;
import dto.Placement;
import org.jooq.DSLContext;

import java.util.function.Supplier;

public class PlacementsPersistence {

    private final Supplier<DSLContext> database;

    public PlacementsPersistence(Supplier<DSLContext> database) {
        this.database = database;
    }

    public void persistPlacement(Placement placement) {
        database.get().executeInsert(toRecord(placement));
    }

    public static PlacementsRecord toRecord(Placement placement) {
        return new PlacementsRecord(
                placement.getUuid(),
                placement.getTimestamp(),
                placement.getSide(),
                placement.getPrice(),
                placement.getSize()
        );
    }
}
