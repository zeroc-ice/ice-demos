// Copyright (c) ZeroC, Inc.

#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>

namespace Time
{
    /// Formats a time point as a string.
    /// @param timePoint The time point.
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
        if (!std::strftime(timeString, sizeof(timeString), "%x %X", &timeInfo))
        {
            throw std::runtime_error("strftime error");
        }

        return timeString;
    }

    /// Converts a time point to a time stamp.
    /// @param timePoint The time point.
    /// @return The time stamp.
    int64_t toTimeStamp(const std::chrono::system_clock::time_point& timePoint)
    {
        const int daysBeforeEpoch = 719162;

        int64_t timeStampMicro = std::chrono::duration_cast<std::chrono::microseconds>(
                                     timePoint.time_since_epoch() + daysBeforeEpoch * std::chrono::hours{24})
                                     .count();

        // The time stamp is in ticks, where each tick is 100 nanoseconds = 0.1 microsecond.
        return timeStampMicro * 10;
    }

    /// Converts a time stamp to a time point.
    /// @param timeStamp The time stamp.
    /// @return The time point.
    std::chrono::system_clock::time_point toTimePoint(int64_t timeStamp)
    {
        const int daysBeforeEpoch = 719162;

        std::chrono::microseconds timePointMicro{timeStamp / 10}; // timeStamp is in ticks (100 nanoseconds)
        return std::chrono::system_clock::time_point{timePointMicro - daysBeforeEpoch * std::chrono::hours{24}};
    }
}

#endif
