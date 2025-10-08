// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>

#include <iostream>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        // Instantiates DataStorm node.
        DataStorm::Node node{argc, argv};

        // Instantiates the "temperature" topic. The topic uses strings for the keys and float for the values.
        DataStorm::Topic<string, float> topic{node, "temperature"};

        // Configure readers to never clear the history. We want to receive all the samples written by the writers.
        topic.setReaderDefaultConfig({std::nullopt, std::nullopt, DataStorm::ClearHistoryPolicy::Never});

        topic.setKeyFilter<string>(
            "startsWith",
            [](string prefix)
            {
                return [prefix = std::move(prefix)](const string& key)
                { return key.size() >= prefix.size() && key.compare(0, prefix.size(), prefix) == 0; };
            });

        // Instantiate a filtered reader for keys starting with the "floor1-" prefix. To monitor the temperature on
        // floor 1.
        auto reader1 = DataStorm::makeFilteredKeyReader(topic, DataStorm::Filter<string>("startsWith", "floor1/"));

        // Create a second filtered reader that uses the built-in _regex filter to monitor the temperature in the
        // main bedrooms.
        auto reader2 = DataStorm::makeFilteredKeyReader(topic, DataStorm::Filter<string>("_regex", ".*/main-bedroom"));

        // Get the samples published by the writer matching the "floor1-" prefix.
        auto sample = reader1.getNextUnread();
        cout << "Reader 1 received " << sample.getKey() << " temperature is " << sample.getValue() << "°C" << endl;

        sample = reader1.getNextUnread();
        cout << "Reader 1 received " << sample.getKey() << " temperature is " << sample.getValue() << "°C" << endl;

        sample = reader1.getNextUnread();
        cout << "Reader 1 received " << sample.getKey() << " temperature is " << sample.getValue() << "°C" << endl;

        // Get the samples published by the writer matching the ".*main-bedroom" regular expression.
        sample = reader2.getNextUnread();
        cout << "Reader 2 received " << sample.getKey() << " temperature is " << sample.getValue() << "°C" << endl;

        sample = reader2.getNextUnread();
        cout << "Reader 2 received " << sample.getKey() << " temperature is " << sample.getValue() << "°C" << endl;
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
