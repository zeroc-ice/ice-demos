// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>

#include <iostream>

using namespace std;

namespace
{

    class LoggerI final : public Ice::Logger
    {
    public:
        void print(const string& message) final { cout << "PRINT: " << message << endl; }

        void trace(const string& category, const string& message) final
        {
            cout << "TRACE(" << category << "): " << message << endl;
        }

        void warning(const string& message) final { cout << "WARNING: " << message << endl; }

        void error(const string& message) final { cout << "ERROR: " << message << endl; }

        string getPrefix() final { return ""; }

        shared_ptr<Ice::Logger> cloneWithPrefix(std::string) final { return make_shared<LoggerI>(); }
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
