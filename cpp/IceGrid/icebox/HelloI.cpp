// Copyright (c) ZeroC, Inc.

#include "HelloI.h"
#include "../../common/Env.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

HelloI::HelloI(string serviceName) : _serviceName(std::move(serviceName)) {}

void
HelloI::sayHello(const Ice::Current&)
{
    const string lang = Env::getEnvironmentVariable("LANG").value_or("en");

    string greeting = "Hello, ";
    if (lang == "fr")
    {
        greeting = "Bonjour, ";
    }
    else if (lang == "de")
    {
        greeting = "Hallo, ";
    }
    else if (lang == "es")
    {
        greeting = "Hola, ";
    }
    else if (lang == "it")
    {
        greeting = "Ciao, ";
    }
    cout << greeting << _serviceName << endl;
}
