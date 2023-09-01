package config;

public class DatabaseConfig {
    private String name;
    private String host;
    private int port;
    private String username;
    private String password;

    public String getName() {
        return name;
    }

    public String getHost() {
        return host;
    }

    public int getPort() {
        return port;
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public String getUrl() {
        return "jdbc:postgresql://"
                .concat(host).concat(":" + port)
                .concat("/").concat(name);
    }
}

