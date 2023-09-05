package dto;

import org.apache.commons.lang3.builder.EqualsBuilder;
import org.apache.commons.lang3.builder.HashCodeBuilder;
import org.apache.commons.lang3.builder.ToStringBuilder;

import java.time.Instant;
import java.util.Optional;
import java.util.UUID;

import static java.util.Optional.empty;
import static java.util.Optional.of;

public class Match {

    private final UUID makerPlacementUuid;
    private final UUID takerPlacementUuid;
    private final Instant timestamp;
    private final long size;

    public Match(UUID makerPlacementUuid, UUID takerPlacementUuid, long size) {
        this.makerPlacementUuid = makerPlacementUuid;
        this.takerPlacementUuid = takerPlacementUuid;
        this.timestamp = Instant.now();
        this.size = size;
    }

    private Match(Builder builder) {
        this.makerPlacementUuid = builder.makerPlacementUuid;
        this.takerPlacementUuid = builder.takerPlacementUuid;
        this.timestamp = builder.timestamp.orElse(Instant.now());
        this.size = builder.size;
    }

    public static Builder match() {
        return new Builder();
    }

    public static class Builder {

        private UUID makerPlacementUuid;
        private UUID takerPlacementUuid;
        private Optional<Instant> timestamp = empty();
        private long size;

        public Builder withMakerPlacementUuid(UUID makerPlacementUuid) {
            this.makerPlacementUuid = makerPlacementUuid;
            return this;
        }

        public Builder withTakerPlacementUuid(UUID takerPlacementUuid) {
            this.takerPlacementUuid = takerPlacementUuid;
            return this;
        }

        public Builder withTimestamp(Instant timestamp) {
            this.timestamp = of(timestamp);
            return this;
        }

        public Builder withSize(long size) {
            this.size = size;
            return this;
        }

        public Match build() {
            return new Match(this);
        }
    }

    public UUID getMakerPlacementUuid() {
        return makerPlacementUuid;
    }

    public UUID getTakerPlacementUuid() {
        return takerPlacementUuid;
    }

    public Instant getTimestamp() {
        return timestamp;
    }

    public long getSize() {
        return size;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;

        if (o == null || getClass() != o.getClass()) return false;

        Match match = (Match) o;

        return new EqualsBuilder()
                .append(size, match.size)
                .append(makerPlacementUuid, match.makerPlacementUuid)
                .append(takerPlacementUuid, match.takerPlacementUuid)
                .append(timestamp, match.timestamp)
                .isEquals();
    }

    @Override
    public String toString() {
        return new ToStringBuilder(this)
                .append("makerPlacementId", makerPlacementUuid)
                .append("takerPlacementId", takerPlacementUuid)
                .append("timestamp", timestamp)
                .append("size", size)
                .toString();
    }
}
