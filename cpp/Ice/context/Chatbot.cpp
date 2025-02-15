// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <iostream>
#include <sstream>

using namespace std;

string
Server::Chatbot::greet(string name, const Ice::Current& current)
{
    // We retrieve the value for the language entry in the context.
    auto p = current.ctx.find("language");
    string language = p != current.ctx.end() ? p->second : "";

    cout << "Dispatching greet request { name = '" << name << "', language = '" << language << "' }" << endl;

    // Return a greeting in the requested language.
    ostringstream os;
    if (language == "de")
    {
        os << "Hallo " << name << ", wie geht's?";
    }
    else if (language == "es")
    {
        os << "¡Hola " << name << "!";
    }
    else if (language == "fr")
    {
        os << "Bonjour " << name << ", comment vas-tu ?";
    }
    else
    {
        os << "Hello, " << name << "!";
    }

    return os.str();
}
