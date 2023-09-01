import cache.Cache;
import com.google.inject.AbstractModule;
import com.google.inject.Provides;
import com.google.inject.Singleton;
import config.Config;
import config.DatabaseConfig;
import config.RedisConfig;
import connectivity.LimitOrderBookFixServerRunner;
import connectivity.LimitOrderBookHttpServerRunner;
import db.Migrator;
import org.jooq.ConnectionProvider;
import org.jooq.DSLContext;
import org.jooq.impl.DSL;
import state.LimitOrderBook;
import state.LimitOrderBookListener;
import state.PersistenceLimitOrderBookListener;

import java.io.IOException;
import java.util.function.Supplier;

import static config.Config.fromPath;

public class ApplicationModule extends AbstractModule {

    private final String configPath;

    public ApplicationModule(String configPath) {
        this.configPath = configPath;
    }

    @Provides
    public Config config() throws IOException {
        return fromPath(configPath);
    }

    @Provides
    public DatabaseConfig databaseConfig(Config config) {
        return config.getDatabaseConfig();
    }

    @Provides
    public Supplier<DSLContext> dslContext(DatabaseConfig databaseConfig) {
        return () -> DSL.using(
                databaseConfig.getUrl(),
                databaseConfig.getUsername(),
                databaseConfig.getPassword()
        );
    }

    @Provides
    public RedisConfig redisConfig(Config config) {
        return config.getRedisConfig();
    }

    @Provides
    public Cache cache(RedisConfig redisConfig) {
        return new Cache(redisConfig);
    }

    @Provides
    public Migrator migrator(DatabaseConfig databaseConfig) {
        return new Migrator(databaseConfig);
    }

    @Provides
    public LimitOrderBookHttpServerRunner httpServerRunner(LimitOrderBook limitOrderBook) {
        return new LimitOrderBookHttpServerRunner(limitOrderBook);
    }

    @Provides
    public LimitOrderBookFixServerRunner fixServerRunner(LimitOrderBook limitOrderBook) {
        return new LimitOrderBookFixServerRunner(limitOrderBook);
    }

    @Provides
    public LimitOrderBookListener listener(Supplier<DSLContext> database, Cache cache) {
        return new PersistenceLimitOrderBookListener(cache, database);
    }

    @Override
    protected void configure() {
    }
}
