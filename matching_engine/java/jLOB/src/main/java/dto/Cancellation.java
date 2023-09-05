package dto;

import org.apache.commons.lang3.builder.ToStringBuilder;

import java.time.Instant;
import java.util.UUID;

import static com.google.common.base.Preconditions.checkArgument;

public class Cancellation {

    private final UUID placementUuid;
    private final Instant timestamp;
    private final long size;

    public Cancellation(UUID placementUuid, long cancellationSize) {
        checkArgument(cancellationSize > 0, "Invalid cancellation size");
        this.timestamp = Instant.now();
        this.placementUuid = placementUuid;
        this.size = cancellationSize;
    }

    public UUID getPlacementUuid() {
        return placementUuid;
    }

    public long getSize() {
        return size;
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    @Override
    public String toString() {
        return new ToStringBuilder(this)
                .append("placementUuid", placementUuid)
                .append("timestamp", timestamp)
                .append("size", size)
                .toString();
    }
}
