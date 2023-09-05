package connectivity.http;

import com.google.gson.*;
import dto.Placement;
import dto.Side;

import java.lang.reflect.Type;

import static dto.Placement.placement;

public class PlacementDeserializer implements JsonDeserializer<Placement> {

    @Override
    public Placement deserialize(JsonElement json, Type typeOfT, JsonDeserializationContext context) throws JsonParseException {
        JsonObject jsonObject = json.getAsJsonObject();
        return placement()
                .withSide(Side.fromString(jsonObject.get("side").getAsString()))
                .withPrice(jsonObject.get("price").getAsLong())
                .withSize(jsonObject.get("size").getAsLong())
                .build();
    }
}
