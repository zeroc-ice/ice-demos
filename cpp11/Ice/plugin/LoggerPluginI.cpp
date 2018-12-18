// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

using namespace std;

namespace
{

class LoggerI : public Ice::Logger
{
public:

    virtual void
    print(const string& message) override
    {
        cout << "PRINT: " << message << endl;
    }

    virtual void
    trace(const string& category, const string& message) override
    {
        cout << "TRACE(" << category << "): " << message << endl;
    }

    virtual void
    warning(const string& message) override
    {
        cout << "WARNING: " << message << endl;
    }

    virtual void
    error(const string& message) override
    {
        cout << "ERROR: " << message << endl;
    }

    virtual string
    getPrefix() override
    {
        return "";
    }

    virtual shared_ptr<Ice::Logger>
    cloneWithPrefix(const std::string&) override
    {
        return make_shared<LoggerI>();
    }
};

}

extern "C"
{

ICE_DECLSPEC_EXPORT ::Ice::Plugin*
createLogger(const shared_ptr<Ice::Communicator>& communicator, const string&, const Ice::StringSeq&)
{
    return new Ice::LoggerPlugin(communicator, make_shared<LoggerI>());
}

}
