// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>

#include <iostream>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        // Instantiates DataStorm node.
        DataStorm::Node node{argc, argv};

        // Instantiates the "hello" topic. The topic uses strings for keys and values.
        DataStorm::Topic<string, string> topic(node, "hello");

        // Instantiate a reader with the key "foo".
        auto reader = DataStorm::makeSingleKeyReader(topic, "foo");

        // Get sample.
        auto sample = reader.getNextUnread();
        cout << sample.getKey() << " says " << sample.getValue() << "!" << endl;
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
