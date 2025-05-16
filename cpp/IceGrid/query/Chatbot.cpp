// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <iostream>
#include <sstream>

using namespace std;

Server::Chatbot::Chatbot(string greeterName) : _greeterName{std::move(greeterName)} {}

string
Server::Chatbot::greet(string name, const Ice::Current&)
{
    cout << "Dispatching greet request { name = '" << name << "' }" << endl;

    ostringstream os;
    os << "Hello, " << name << "! My name is " << _greeterName << ". How are you today?";

    return os.str();
}
