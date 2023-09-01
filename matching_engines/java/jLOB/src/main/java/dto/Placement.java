package dto;

import org.apache.commons.lang3.builder.ToStringBuilder;

import java.io.Serializable;
import java.time.Instant;
import java.util.Optional;
import java.util.UUID;

import static com.google.common.base.Preconditions.checkArgument;
import static java.util.Optional.empty;
import static java.util.Optional.of;

public class Placement implements Serializable {

    private final UUID uuid;
    private final Instant timestamp;
    private final Side side;
    private final long price;
    private long size;

    public static Builder placement() {
        return new Builder();
    }

    public Placement copy() {
        return placement()
                .withUuid(this.uuid)
                .withInstant(this.timestamp)
                .withSide(this.side)
                .withPrice(this.price)
                .withSize(this.size)
                .build();
    }

    private Placement(Builder builder) {
        checkArgument(builder.price > 0, "Invalid placement price");
        checkArgument(builder.size > 0, "Invalid placement size");
        this.uuid = builder.uuid.orElse(UUID.randomUUID());
        this.timestamp = builder.timestamp.orElse(Instant.now());
        this.side = builder.side;
        this.price = builder.price;
        this.size = builder.size;
    }

    public static class Builder {

        private Optional<UUID> uuid = empty();
        private Optional<Instant> timestamp = empty();
        private Side side;
        private long price;
        private long size;

        private Builder withUuid(UUID uuid) {
            this.uuid = of(uuid);
            return this;
        }

        private Builder withInstant(Instant timestamp) {
            this.timestamp = of(timestamp);
            return this;
        }

        public Builder withSide(Side side) {
            this.side = side;
            return this;
        }

        public Builder withPrice(long price) {
            this.price = price;
            return this;
        }

        public Builder withSize(long size) {
            this.size = size;
            return this;
        }

        public Placement build() {
            return new Placement(this);
        }
    }

    public UUID getUuid() {
        return uuid;
    }

    public long getSize() {
        return size;
    }

    public long getPrice(){
        return price;
    }

    public Side getSide() {
        return side;
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    public void reduce(long size){
        this.size -= size;
    }

    @Override
    public String toString() {
        return new ToStringBuilder(this)
                .append("id", uuid)
                .append("timestamp", timestamp)
                .append("side", getSide())
                .append("price", getPrice())
                .append("size", size)
                .toString();
    }
}