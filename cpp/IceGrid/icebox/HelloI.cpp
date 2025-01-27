// Copyright (c) ZeroC, Inc.

#ifdef _MSC_VER
// For getenv
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "HelloI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

HelloI::HelloI(string serviceName) : _serviceName(std::move(serviceName)) {}

void
HelloI::sayHello(const Ice::Current&)
{
    char* val = getenv("LANG");
    const string lang = val ? string(val) : "en";

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
