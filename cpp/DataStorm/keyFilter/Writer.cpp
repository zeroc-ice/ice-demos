// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"

#include <DataStorm/DataStorm.h>

#include <iostream>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Instantiates DataStorm node.
    DataStorm::Node node{argc, argv};

    // Instantiates the "temperature" topic. The topic uses strings for the keys and float for the values.
    DataStorm::Topic<string, float> topic{node, "temperature"};

    // Define the same custom key filter as the reader. A key filter must be registered on the writer's topic too: the
    // writer resolves the filter a reader announces against its own factories, and a filter it can't resolve matches
    // all the keys.
    topic.setKeyFilter<string>(
        "startsWith",
        [](string prefix)
        {
            return [prefix = std::move(prefix)](const string& key)
            { return key.size() >= prefix.size() && key.compare(0, prefix.size(), prefix) == 0; };
        });

    // Instantiate a writer for the rooms this application publishes. The writer evaluates the readers' key filters
    // against these keys and sends a sample only to the readers whose filter matches its key.
    auto writer = DataStorm::makeMultiKeyWriter(
        topic,
        {"floor1/main-bedroom",
         "floor1/secondary-bedroom",
         "floor1/studio",
         "floor2/main-bedroom",
         "floor2/secondary-bedroom",
         "floor2/studio"},
        "temperature-writer");

    // Wait for a reader to connect
    topic.waitForReaders();

    // Publish temperature samples for various rooms in the house.
    writer.update("floor1/main-bedroom", 21.5f);
    writer.update("floor1/secondary-bedroom", 22.0f);
    writer.update("floor1/studio", 21.8f);
    writer.update("floor2/main-bedroom", 23.3f);
    writer.update("floor2/secondary-bedroom", 23.1f);
    writer.update("floor2/studio", 22.9f);

    // Wait for readers to disconnect.
    topic.waitForNoReaders();
    return 0;
}
