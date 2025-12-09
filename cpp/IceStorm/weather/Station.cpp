// Copyright (c) ZeroC, Inc.

#include "ConsolePrinter.h"

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>
#include <iostream>

using namespace ClearSky;
using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create an object adapter.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create an object adapter that listens for incoming requests and dispatches them to servants. This object adapter
    // listens on an OS-assigned TCP port, on all interfaces.
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("StationAdapter", "tcp");

    // Register the ConsolePrinter servant with the adapter, and get a proxy to the new object.
    // We use a UUID for the identity because these subscribers (weather stations) are transient: if this program exits
    // without unsubscribing its weather station, when it restarts, it logically creates a new weather station as
    // opposed to re-incarnating the old one.
    // See also the Retry Count QoS section in the IceStorm documentation.
    auto weatherStation = adapter->addWithUUID<WeatherStationPrx>(make_shared<Server::ConsolePrinter>());

    // Create a proxy to the IceStorm topic manager.
    IceStorm::TopicManagerPrx topicManager{communicator, "ClearSky/TopicManager:tcp -p 4061 -h localhost"};

    // Ask the topic manager to create or retrieve the "weather" topic and return the corresponding proxy.
    std::optional<IceStorm::TopicPrx> topic = topicManager->createOrRetrieve("weather");

    // The proxy returned by createOrRetrieve is never null.
    assert(topic);

    // Start dispatching requests.
    adapter->activate();
    cout << "Listening..." << endl;

    // Register our weather station with the topic.
    // subscribeAndGetPublisher returns a publisher proxy that we don't need here.
    topic->subscribeAndGetPublisher({}, weatherStation);
    std::cout << "Subscribed weather station to topic '" << topicName << "'." << std::endl;

    // Shut down the communicator when the user presses Ctrl+C.
    ctrlCHandler.setCallback(
        [communicator](int)
        {
            cout << "Caught signal, shutting down..." << endl;
            communicator->shutdown(); // starts shutting down
        });

    // Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
    communicator->waitForShutdown();

    // Unsubscribe from the topic. The shutdown above only shuts down the object adapter. All client-side
    // functionalities remain available until the communicator is disposed.
    topic->unsubscribe(weatherStation);
    std::cout << "Unsubscribed weather station from topic '" << topicName << "', exiting..." << std::endl;

    return 0;
}
