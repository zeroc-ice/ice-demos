// Copyright (c) ZeroC, Inc.

#include <cstdlib>
#include <optional>
#include <string>

namespace Env
{
    /// Gets the value of an environment variable.
    /// @param variableName The name of the environment variable.
    /// @return The value of the environment variable, or nullopt if the variable is not set.
    std::optional<std::string> getEnvironmentVariable(const char* variableName)
    {
#ifdef _WIN32
        size_t requestedSize;
        char buffer[256];
        if (getenv_s(&requestedSize, buffer, sizeof(buffer), variableName))
        {
            return std::nullopt;
        }
        const char* value = requestedSize > 0 ? buffer : nullptr;
#else
        const char* value = getenv(variableName);
#endif
        return value ? std::optional<std::string>(value) : std::nullopt;
    }

    /// Get the username of the current user.
    /// @return The username of the current user.
    std::string getUsername()
    {
#ifdef _WIN32
        const std::optional<std::string> name = Env::getEnvironmentVariable("USERNAME");
#else
        const std::optional<std::string> name = Env::getEnvironmentVariable("USER");
#endif
        return name ? *name : "masked user";
    }
}
