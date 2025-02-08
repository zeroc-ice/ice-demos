// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace ConfigServer;

string
Chatbot::greet(string name, const Ice::Current&)
{
    cout << "Dispatching greet request { name = '" << name << "' }" << endl;

    ostringstream os;
    os << "Hello, " << name << "!";
    return os.str();
}
