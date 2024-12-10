// Copyright (c) ZeroC, Inc. All rights reserved.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>
#include <optional>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        // Instantiates DataStorm node.
        DataStorm::Node node(argc, argv, "config.reader");

        // Instantiates the "hello" topic. The topic uses strings for keys and values.
        DataStorm::Topic<string, string> topic(node, "hello");

        // Configure readers to never clear the history. We want to receive all the
        // samples written by the writers.
        topic.setReaderDefaultConfig({std::nullopt, std::nullopt, DataStorm::ClearHistoryPolicy::Never});

        //
        // Instantiate a filtered reader for keys matching the foo[ace] regular expression.
        //
        auto reader = DataStorm::makeFilteredKeyReader(topic, DataStorm::Filter<string>("_regex", "foo[ace]"));

        // Get the samples published by the writers fooa, fooc and fooe.
        auto sample = reader.getNextUnread();
        cout << sample.getKey() << " says " << sample.getValue() << "!" << endl;

        sample = reader.getNextUnread();
        cout << sample.getKey() << " says " << sample.getValue() << "!" << endl;

        sample = reader.getNextUnread();
        cout << sample.getKey() << " says " << sample.getValue() << "!" << endl;
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
