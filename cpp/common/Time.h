// Copyright (c) ZeroC, Inc.

#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <iostream>
#include <string>

namespace Time
{
    /// Format a time point as a string.
    /// @return The formatted time point.
    inline std::string formatTime(std::chrono::system_clock::time_point timePoint)
    {
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        std::tm timeInfo;

#ifdef _WIN32
        if (localtime_s(&timeInfo, &time))
        {
            throw std::runtime_error("localtime_s error");
        }
#else
        if (!localtime_r(&time, &timeInfo))
        {
            throw std::runtime_error("localtime_r error");
        }
#endif
        char timeString[100];
        if (!strftime(timeString, sizeof(timeString), "%x %X", &timeInfo))
        {
            throw std::runtime_error("strftime error");
        }

        return timeString;
    }
}

#endif
