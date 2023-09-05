import com.google.inject.Guice;
import com.google.inject.Injector;
import connectivity.LimitOrderBookFixServerRunner;
import connectivity.LimitOrderBookHttpServerRunner;
import db.Migrator;


public class Application {
    public static void main(String[] args)  {
        try {
            Injector injector = Guice.createInjector(new ApplicationModule(args[0]));
            injector.getInstance(Migrator.class).migrate();
            injector.getInstance(LimitOrderBookHttpServerRunner.class).run();
            injector.getInstance(LimitOrderBookFixServerRunner.class).run();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }
}
