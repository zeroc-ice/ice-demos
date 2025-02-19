// Copyright (c) ZeroC, Inc.

#include <optional>
#include <string>

namespace common
{
    /// Get the value of an environment variable.
    /// @param variableName The name of the environment variable.
    /// @return The value of the environment variable, or nullopt if the variable is not set.
    std::optional<std::string> getEnvironmentVariable(const char* variableName)
    {
#ifdef _MSC_VER
        size_t requestedSize;
        char buffer[256];
        if (getenv_s(&requestedSize, buffer, sizeof(buffer), variableName))
        {
            // getenv_s failed, set requestedSize to 0 to indicate that the buffer is not valid.
            requestedSize = 0;
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
#ifdef _MSC_VER
        const auto name = common::getEnvironmentVariable("USERNAME");
#else
        const auto name = common::getEnvironmentVariable("USER");
#endif
        return name ? *name : "masked user";
    }
}
