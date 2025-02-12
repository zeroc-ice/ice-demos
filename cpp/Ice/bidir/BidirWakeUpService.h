// Copyright (c) ZeroC, Inc.

#ifndef BIDIR_WAKE_UP_SERVICE_H
#define BIDIR_WAKE_UP_SERVICE_H

#include "AlarmClock.h"

#include <future>
#include <vector>

namespace Server
{
    /// BidirWakeUpService is an Ice servant that implements Slice interface WakeUpService.
    class BidirWakeUpService : public EarlyRiser::WakeUpService
    {
    public:
        // Waits for all outstanding tasks to complete.
        ~BidirWakeUpService() override;

        // Implements the pure virtual function in the base class (WakeUpService) generated by the Slice compiler.
        void wakeMeUp(std::int64_t timeStamp, const Ice::Current&) override;

    private:
        std::vector<std::future<void>> _tasks;
    };
}

#endif
