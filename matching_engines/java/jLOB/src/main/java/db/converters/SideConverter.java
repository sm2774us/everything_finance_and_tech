package db.converters;

import dto.Side;
import org.jooq.impl.EnumConverter;

public class SideConverter extends EnumConverter<String, Side> {

    public SideConverter() {
        super(String.class, Side.class);
    }
}
