// Copyright (c) ZeroC, Inc.

#include <chrono>
#include <iostream>
#include <string>

namespace common
{
    /// Format a time point as a string.
    /// @return The formatted time point.
    std::string formatTime(std::chrono::system_clock::time_point timePoint)
    {
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        std::tm timeInfo;

#ifdef _MSC_VER
        if (localtime_s(&timeInfo, &time))
        {
            std::cerr << "localtime_s error" << std::endl;
            return {};
        }
#else
        if (!localtime_r(&time, &timeInfo))
        {
            std::cerr << "localtime_r error" << std::endl;
            return {};
        }
#endif
        char timeString[100];
        if (!strftime(timeString, sizeof(timeString), "%x %X", &timeInfo))
        {
            std::cerr << "strftime error" << std::endl;
            return {};
        }

        return timeString;
    }
}
