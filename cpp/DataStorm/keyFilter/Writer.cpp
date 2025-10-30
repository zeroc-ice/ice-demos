// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>

#include <iostream>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    // Instantiates DataStorm node.
    DataStorm::Node node{argc, argv};

    // Instantiates the "temperature" topic. The topic uses strings for the keys and float for the values.
    DataStorm::Topic<string, float> topic{node, "temperature"};

    // Instantiate a any key writer.
    auto writer = DataStorm::makeAnyKeyWriter(topic, "temperature-writer");

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
