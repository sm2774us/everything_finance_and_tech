-- placements
CREATE TABLE IF NOT EXISTS placements (
    uuid uuid not null,
    timestamp timestamp not null,
    side varchar(5) not null,
    price bigint not null,
    size bigint not null
);

CREATE INDEX placements_timestamp_idx ON placements (timestamp desc);

-- cancellations
CREATE TABLE IF NOT EXISTS cancellations (
    placement_uuid uuid not null,
    timestamp timestamp not null,
    size bigint not null
);

CREATE INDEX cancellations_timestamp_idx ON cancellations (timestamp desc);

-- matches
CREATE TABLE IF NOT EXISTS matches (
    maker_uuid uuid not null,
    taker_uuid uuid not null,
    timestamp timestamp not null,
    size bigint not null
);

CREATE INDEX matches_timestamp_idx ON matches (timestamp desc);