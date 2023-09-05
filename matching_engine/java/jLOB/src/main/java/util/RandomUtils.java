package util;

import java.util.Random;

public class RandomUtils {

    private static final Random RND = new Random();

    private static <T extends Enum<?>> T randomEnum(Class<T> cls){
        int x = RND.nextInt(cls.getEnumConstants().length);
        return cls.getEnumConstants()[x];
    }
}
