//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <HelloI.h>
#include <future>

using namespace std;

int main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceUDP();
    Ice::registerIceWS();
#endif

    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started.
        //
        promise<void> requestShutdownPromise;
        future<void> shutdownRequested = requestShutdownPromise.get_future();
        auto requestShutdown = [&] { requestShutdownPromise.set_value(); };
        Ice::CtrlCHandler ctrlCHandler([&] (int) { requestShutdown(); });

        //
        // CommunicatorHolder's ctor initializes an Ice communicator and its dtor destroys this communicator.
        // The destroy operation shuts down the communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.server");
        const auto& communicator = ich.communicator();

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv.
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            auto adapter = communicator->createObjectAdapter("Hello");
            adapter->add(make_shared<HelloI>(requestShutdown), Ice::stringToIdentity("hello"));
            adapter->activate();

            // Wait until Hello::shutdown or the CtrlCHandler call requestShutdown.
            shutdownRequested.wait();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
