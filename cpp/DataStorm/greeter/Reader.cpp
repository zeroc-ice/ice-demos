// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create a DataStorm node using the provided command-line arguments.
    DataStorm::Node node{argc, argv};

    // Create a topic named "greeter" with string keys and string values.
    DataStorm::Topic<string, string> topic{node, "greeter"};

    // Create a reader subscribed to all keys (an any-key reader).
    auto reader = DataStorm::makeAnyKeyReader(topic);

    // Read the next unread sample (blocks until one is available).
    auto sample = reader.getNextUnread();
    cout << sample.getKey() << " says " << sample.getValue() << endl;
    return 0;
}
