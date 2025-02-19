// Copyright (c) ZeroC, Inc.

#ifndef ENV_H
#define ENV_H

#include <cstdlib>
#include <optional>
#include <string>

namespace Env
{
    /// Gets the value of an environment variable.
    /// @param variableName The name of the environment variable.
    /// @return The value of the environment variable, or nullopt if the variable is not set.
    inline std::optional<std::string> getEnvironmentVariable(const char* variableName)
    {
#ifdef _WIN32
        size_t requiredSize;
        char buffer[256];
        if (getenv_s(&requiredSize, buffer, sizeof(buffer), variableName))
        {
            return std::nullopt;
        }
        const char* value = requiredSize > 0 ? buffer : nullptr;
#else
        const char* value = std::getenv(variableName);
#endif
        return value ? std::optional<std::string>(value) : std::nullopt;
    }

    /// Get the username of the current user.
    /// @return The username of the current user.
    inline std::string getUsername()
    {
#ifdef _WIN32
        const std::optional<std::string> name = Env::getEnvironmentVariable("USERNAME");
#else
        const std::optional<std::string> name = Env::getEnvironmentVariable("USER");
#endif
        return name ? *name : "masked user";
    }
}

#endif
