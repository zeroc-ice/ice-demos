// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

HelloI::HelloI(const string& serviceName) :
    _serviceName(serviceName)
{
}

void
HelloI::sayHello(const Ice::Current&)
{
#ifdef _WIN32
    vector<wchar_t> buf;
    buf.resize(1024);
    DWORD val = GetEnvironmentVariableW(L"LANG", &buf[0], static_cast<DWORD>(buf.size()));
    string lang = (val > 0 && val < buf.size()) ?
        wstringToString(&buf[0], Ice::getProcessStringConverter()) : string("en");
#else
    char* val = getenv("LANG");
    string lang = val ? string(val) : "en";
#endif

    string greeting = "Hello, ";
    if(lang == "fr")
    {
        greeting = "Bonjour, ";
    }
    else if(lang == "de")
    {
        greeting = "Hallo, ";
    }
    else if(lang == "es")
    {
        greeting = "Hola, ";
    }
    else if(lang == "it")
    {
        greeting = "Ciao, ";
    }
    cout << greeting << _serviceName << endl;
}
