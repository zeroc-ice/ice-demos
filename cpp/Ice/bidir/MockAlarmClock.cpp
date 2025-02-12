// Copyright (c) ZeroC, Inc.

#include "MockAlarmClock.h"

#include <iostream>

using namespace EarlyRiser;
using namespace std;

ButtonPressed
Client::MockAlarmClock::ring(string message, const Ice::Current&)
{
    cout << "Dispatching ring request { message = '" << message << "' }" << endl;

    if (_needMoreTime)
    {
        cout << "Returning " << ButtonPressed::Snooze << " to request more time." << endl;
        _needMoreTime = false; // we only snooze one time
        return ButtonPressed::Snooze;
    }
    else
    {
        _stopPromise.set_value();
        cout << "Returning " << ButtonPressed::Stop << " to stop the alarm." << endl;
        return ButtonPressed::Stop;
    }
}

void
Client::MockAlarmClock::wait()
{
    _stopPromise.get_future().wait();
}
