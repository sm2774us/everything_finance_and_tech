#include <core/compiler/ignored_warnings.h>
// COMPILE TIME CHECKS
#include <core/compiler/cpp_version_check.h> // C++11 and later supported
#include <core/compiler/os_version_check.h>  // Linux and Windows supported

// RUNTIME CHECKS
#include <core/memory/cpu_memory.h>  // To see if cache line we are running on
                                     // matches the compiled one

#include <memory>

#include <core/self_process.h>
#include <core/single_instance.h>

#include <core/logger/logger.h>
#include <core/file_utility.h>

#include <server/server_configuration.h>
#include <server/server_offline.h>
#include <server/server_fix.h>

#include <server/server_error.h>
#include <server/server_constants.h>

using namespace std;

int main ()
{
    // Initial checks
    if (core::getCPUCacheLineSize() != CACHE_LINE_SIZE)
    {
        auto message = core::format("This executable compiled for cache line size %d , but you are running on a CPU with a cache line of %d", CACHE_LINE_SIZE, core::getCPUCacheLineSize());
        onError(ServerError::NON_SUPPORTED_EXECUTION, message);
    }

    // Single instance protection
    core::SingleInstance singleInstance;
    if (singleInstance() == false )
    {
        onError(ServerError::ALREADY_RUNNING);
    }

    if (core::SelfProcess::amIAdmin() == false)
    {
        // Mainly needed for ability to set thread priorities
        core::consoleOutputWithColor(core::ConsoleColor::FG_RED, " WARNING : Program didn`t start with admin/root rights. Therefore will not be able to modify thread priorities.\n");
    }

    // Load configuration file
    ServerConfiguration serverConfiguration;

    try
    {
        // Set current working directory as current executable`s directory
        core::SelfProcess::setCurrentWorkingDirectory(core::SelfProcess::getCurrentExecutableDirectory());

        serverConfiguration.load(server_constants::CONFIGURATION_FILE);

        // Set process priority
        core::SelfProcess::setPriority(core::SelfProcess::getProcessPriorityFromString(serverConfiguration.getProcessPriority()));

        // Start logger if enabled
        core::Logger::getInstance()->initialise(serverConfiguration.getLoggerConfiguration());
        core::Logger::getInstance()->start();
        LOG_INFO("Main thread", "starting")

        unique_ptr<ServerBase> server;

        if (serverConfiguration.getOrderEntryMode() == "FIX")
        {
            // FIX SERVER MODE
            server.reset(new ServerFix(serverConfiguration));
        }
        else
        {
            // OFFLINE ORDER ENTRY MODE
            server.reset(new ServerOffline(serverConfiguration));
        }

        server->run();

    }
    catch (const std::invalid_argument & e)
    {
        onError(ServerError::RUNTIME_ERROR, e.what());
    }
    catch (const std::runtime_error & e)
    {
        onError(ServerError::RUNTIME_ERROR, e.what());
    }
    catch (const std::bad_alloc &)
    {
        onError(ServerError::INSUFFICIENT_MEMORY);
    }
    catch (...)
    {
        onError(ServerError::UNKNOWN_PROBLEM);
    }

    // Application exit
    LOG_INFO("Main thread", "Ending")
    core::Logger::getInstance()->shutdown();
    return 0;
}