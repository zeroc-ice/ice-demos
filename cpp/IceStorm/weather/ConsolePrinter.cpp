// Copyright (c) ZeroC, Inc.

#include "ConsolePrinter.h"

#include <iostream>

using namespace std;

void
Server::ConsolePrinter::report(
    string sensorId,
    string timeStamp,
    ClearSky::AtmosphericConditionsPtr reading,
    const Ice::Current&)
{
    cout << sensorId << " measured " << reading << " at " << timeStamp << endl;
}
