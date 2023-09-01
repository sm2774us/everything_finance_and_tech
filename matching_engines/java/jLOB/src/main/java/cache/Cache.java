package cache;

import com.google.inject.Inject;
import config.RedisConfig;
import org.redisson.Redisson;
import org.redisson.api.RBucket;
import org.redisson.api.RedissonClient;
import org.redisson.config.Config;
import state.LimitOrderBook;

public class Cache {

    private final RBucket<LimitOrderBook> limitOrderBookBucket;

    @Inject
    public Cache(RedisConfig redisConfig) {
        Config config = new Config();
        config.useSingleServer()
                .setAddress("redis://" + redisConfig.getHost() + ":" + redisConfig.getPort());

        RedissonClient cache = Redisson.create(config);
        this.limitOrderBookBucket = cache.getBucket("book");
    }

    public void cacheLimitOrderBook(LimitOrderBook limitOrderBook) {
        limitOrderBookBucket.set(limitOrderBook);
    }

    public LimitOrderBook getLimitOrderBook() {
        return limitOrderBookBucket.get();
    }

    public boolean bookKeyExists() {
        return limitOrderBookBucket.isExists();
    }
}
