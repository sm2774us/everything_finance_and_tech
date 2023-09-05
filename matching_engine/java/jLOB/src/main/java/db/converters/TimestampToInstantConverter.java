package db.converters;

import org.jooq.Converter;

import java.sql.Timestamp;
import java.time.Instant;

public class TimestampToInstantConverter implements Converter<Timestamp, Instant> {

    @Override
    public Instant from(Timestamp databaseObject) {
        return databaseObject.toInstant();
    }

    @Override
    public Timestamp to(Instant userObject) {
        return Timestamp.from(userObject);
    }

    @Override
    public Class<Timestamp> fromType() {
        return Timestamp.class;
    }

    @Override
    public Class<Instant> toType() {
        return Instant.class;
    }
}
