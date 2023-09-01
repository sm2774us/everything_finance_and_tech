package connectivity.http;

import com.google.gson.*;
import dto.Cancellation;

import java.lang.reflect.Type;
import java.util.UUID;

public class CancellationDeserializer implements JsonDeserializer<Cancellation> {

    @Override
    public Cancellation deserialize(JsonElement json, Type typeOfT, JsonDeserializationContext context) throws JsonParseException {
        JsonObject jsonObject = json.getAsJsonObject();
        return new Cancellation(
                UUID.fromString(jsonObject.get("uuid").getAsString()),
                jsonObject.get("size").getAsLong()
        );
    }
}
