// Copyright (c) ZeroC, Inc.

#include "ConsolePrinter1.h"

#include <iostream>

using namespace std;

void
Server::ConsolePrinter::report(string sensorId, ClearSky::AtmosphericConditionsPtr reading, const Ice::Current&)
{
    cout << sensorId << " reported " << reading << " to station v1" << endl;
}
