package db;

import com.google.inject.Inject;
import config.DatabaseConfig;
import org.flywaydb.core.Flyway;


public class Migrator {

    private final DatabaseConfig databaseConfig;

    @Inject
    public Migrator(DatabaseConfig databaseConfig) {
        this.databaseConfig = databaseConfig;
    }

    public void migrate() {
        Flyway flyway = Flyway.configure().dataSource(
                databaseConfig.getUrl(),
                databaseConfig.getUsername(),
                databaseConfig.getPassword()
        ).load();
        flyway.migrate();
    }
}
