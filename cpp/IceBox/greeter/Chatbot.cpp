// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <iostream>
#include <sstream>

using namespace std;

GreeterServer::Chatbot::Chatbot(string chatbotName) : _chatbotName(std::move(chatbotName)) {}

string
GreeterServer::Chatbot::greet(string name, const Ice::Current&)
{
    cout << "Dispatching greet request { name = '" << name << "' }" << endl;

    ostringstream os;
    os << "Hello, " << name << "! My name is " << _chatbotName << ". How can I help you today?";
    return os.str();
}
