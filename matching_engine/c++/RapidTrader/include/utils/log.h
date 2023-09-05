#ifndef RAPID_TRADER_LOG_H
#define RAPID_TRADER_LOG_H
#include <spdlog/spdlog.h>

class Log
{
public:
    /**
     * Initializes the logger.
     */
    static void init();

    /**
     * @return a reference to the logger.
     */
    inline static std::shared_ptr<spdlog::logger> &getLogger()
    {
        return logger;
    }

private:
    static std::shared_ptr<spdlog::logger> logger;
};

#ifndef NDEBUG
#    define LOG_TRACE(...) Log::getLogger()->trace(__VA_ARGS__)
#    define LOG_DEBUG(...) Log::getLogger()->debug(__VA_ARGS__)
#    define LOG_INFO(...) Log::getLogger()->info(__VA_ARGS__)
#    define LOG_WARN(...) Log::getLogger()->warn(__VA_ARGS__)
#    define LOG_ERROR(...) Log::getLogger()->error(__VA_ARGS__)
#    define LOG_CRITICAL(...) Log::getLogger()->critical(__VA_ARGS__)
#else
#    define LOG_TRACE
#    define LOG_DEBUG
#    define LOG_INFO
#    define LOG_WARN
#    define LOG_ERROR(...) Log::getLogger()->error(__VA_ARGS__)
#    define LOG_CRITICAL(...) Log::getLogger()->critical(__VA_ARGS__)
#endif
#endif // RAPID_TRADER_LOG_H
