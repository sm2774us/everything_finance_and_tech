#ifndef _SERVER_ERROR_H_
#define _SERVER_ERROR_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include <type_traits>
#include <core/logger/logger.h>

enum class ServerError {
    ALREADY_RUNNING,
    RUNTIME_ERROR,
    INSUFFICIENT_MEMORY,
    NON_SUPPORTED_EXECUTION,
    UNKNOWN_PROBLEM
};

inline void onError(ServerError error, const std::string& detail = "")
{
    std::string errorMessage;

    switch (error)
    {
        case ServerError::ALREADY_RUNNING:
            errorMessage = "Process is already running";
            break;

        case ServerError::RUNTIME_ERROR:
            errorMessage = detail;
            break;

        case ServerError::INSUFFICIENT_MEMORY:
            errorMessage = "Insufficient memory";
            break;

        case ServerError::NON_SUPPORTED_EXECUTION:
            errorMessage = detail;
            break;

        case ServerError::UNKNOWN_PROBLEM:
            errorMessage = "Unknown exception occured";
            break;
    }

    if (core::Logger::getInstance()->isAlive())
    {
        LOG_ERROR("Server", errorMessage.c_str())
    }

    std::cerr << errorMessage << std::endl;

    auto exit_code = static_cast<std::underlying_type<ServerError>::type >(error);
    std::exit(exit_code);
}

#endif