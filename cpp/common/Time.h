// Copyright (c) ZeroC, Inc.

#include <chrono>
#include <iostream>
#include <string>

namespace Time
{
    /// Format a time point as a string.
    /// @return The formatted time point.
    std::string formatTime(std::chrono::system_clock::time_point timePoint)
    {
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        std::tm timeInfo;

#ifdef _WIN32
        if (localtime_s(&timeInfo, &time))
        {
            throw runtime_error("localtime_s error");
        }
#else
        if (!localtime_r(&time, &timeInfo))
        {
            throw runtime_error("localtime_r error");
        }
#endif
        char timeString[100];
        if (!strftime(timeString, sizeof(timeString), "%x %X", &timeInfo))
        {
            throw runtime_error("strftime error");
        }

        return timeString;
    }
}
