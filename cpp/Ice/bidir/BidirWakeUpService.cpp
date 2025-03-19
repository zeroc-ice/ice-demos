// Copyright (c) ZeroC, Inc.

#include "BidirWakeUpService.h"
#include "../../common/Time.h"

#include <algorithm>
#include <iostream>

using namespace EarlyRiser;
using namespace std;

Server::BidirWakeUpService::~BidirWakeUpService()
{
    // Waits for all outstanding tasks to complete.
    for (const auto& task : _tasks)
    {
        task.wait();
    }
}

void
Server::BidirWakeUpService::wakeMeUp(int64_t timeStamp, const Ice::Current& current)
{
    cout << "Dispatching wakeMeUp request { timeStamp = " << timeStamp << " ticks }" << endl;

    chrono::system_clock::time_point timePoint = Time::toTimePoint(timeStamp);

    Ice::ConnectionPtr connection = current.con; // The connection from the client to the server.
    if (!connection)
    {
        // Should never happen, but in case it does, the Ice runtime will send an Ice::UnknownException to the client.
        throw std::invalid_argument{"BidirWakeUpService does not support collocated calls"};
    }

    // Create a proxy to the client's alarm clock. This connection-bound proxy is called a "fixed proxy".
    auto alarmClock = connection->createProxy<AlarmClockPrx>(Ice::stringToIdentity("alarmClock"));

    // Schedule a wake-up call in a background task.
    _tasks.emplace_back(std::async(
        std::launch::async,
        [alarmClock = std::move(alarmClock), timePoint]()
        {
            // Sleep until the specified time point.
            this_thread::sleep_until(timePoint);

            // First ring.
            ButtonPressed buttonPressed = alarmClock->ring("It's time to wake up!");

            // Keep ringing every 10 seconds until the user presses the stop button.
            while (buttonPressed == ButtonPressed::Snooze)
            {
                this_thread::sleep_for(10s);
                buttonPressed = alarmClock->ring("No more snoozing!");
            }
        }));

    // We don't want the _tasks vector to grow forever so remove all completed tasks here, without waiting.
    _tasks.erase(
        std::remove_if(
            _tasks.begin(),
            _tasks.end(),
            [](const auto& task) { return task.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }),
        _tasks.end());
}
