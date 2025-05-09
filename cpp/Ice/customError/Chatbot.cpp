// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"
#include "../../common/Time.h"

#include <Ice/Ice.h>
#include <iostream>
#include <sstream>

using namespace std;

string
Server::Chatbot::greet(string name, const Ice::Current&)
{
    cout << "Dispatching greet request { name = '" << name << "' }" << endl;

    if (name.empty())
    {
        // ObjectNotExistException is a dispatch exception with a reply status of 'ObjectNotExist'.
        throw Ice::ObjectNotExistException{__FILE__, __LINE__};
    }
    else if (name == "alice")
    {
        // This is a bogus error since there is no authentication in this demo.
        throw Ice::DispatchException{
            __FILE__,
            __LINE__,
            Ice::ReplyStatus::Unauthorized,
            "Invalid credentials. The administrator has been notified."};
    }
    else if (name == "bob")
    {
        auto timeString = Time::formatTime(chrono::system_clock::now() + chrono::minutes(5));
        throw VisitorCenter::GreeterException{"Away until " + timeString + ".", VisitorCenter::GreeterError::Away};
    }
    else if (name == "carol")
    {
        throw VisitorCenter::GreeterException{
            "I am already greeting someone else.",
            VisitorCenter::GreeterError::GreetingOtherVisitor};
    }
    else if (name.length() > _maxLength)
    {
        throw VisitorCenter::GreeterException("Name is longer than maximum!", VisitorCenter::GreeterError::NameTooLong);
    }

    return "Hello, " + name + "!";
}
