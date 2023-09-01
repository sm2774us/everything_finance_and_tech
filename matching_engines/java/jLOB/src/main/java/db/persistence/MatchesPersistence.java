package db.persistence;

import db.jooq.tables.records.MatchesRecord;
import dto.Match;
import org.jooq.DSLContext;

import java.util.function.Supplier;

public class MatchesPersistence {

    private final Supplier<DSLContext> database;

    public MatchesPersistence(Supplier<DSLContext> database) {
        this.database = database;
    }

    public void persistMatch(Match match) {
        database.get().executeInsert(toRecord(match));
    }

    public static MatchesRecord toRecord(Match match) {
        return new MatchesRecord(
                match.getMakerPlacementUuid(),
                match.getTakerPlacementUuid(),
                match.getTimestamp(),
                match.getSize()
        );
    }
}
