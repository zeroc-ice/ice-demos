// Copyright (c) ZeroC, Inc.

#include "../../common/Time.h"

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <chrono>
#include <iostream>

using namespace std;

namespace DataStorm
{
    template<> struct Encoder<chrono::system_clock::time_point>
    {
        static Ice::ByteSeq encode(const chrono::system_clock::time_point&)
        {
            assert(false); // Not used by the reader but it still needs to be declared.
            return Ice::ByteSeq{};
        }
    };

    template<> struct Decoder<chrono::system_clock::time_point>
    {
        static chrono::system_clock::time_point decode(const Ice::ByteSeq& data)
        {
            // Decode the number of seconds since epoch. The value is encoded in a way which doesn't depend on the
            // platform endianess (little endian with variable number of bytes).
            long long int value = 0;
            for (auto p = data.rbegin(); p != data.rend(); ++p)
            {
                value = value * 256 + static_cast<long long int>(*p);
            }
            return chrono::time_point<chrono::system_clock>(chrono::seconds(value));
        }
    };
};

int
main(int argc, char* argv[])
{
    try
    {
        // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the
        // beginning of the program, before creating a DataStorm node or starting any thread.
        Ice::CtrlCHandler ctrlCHandler;

        // Instantiates node.
        DataStorm::Node node{argc, argv};

        // Shutdown the node on Ctrl-C.
        ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

        // Instantiates the "time" topic.
        DataStorm::Topic<string, chrono::system_clock::time_point> topic{node, "time"};

        // Instantiate a reader to read the time from all the topic cities.
        auto reader = DataStorm::makeAnyKeyReader(topic);

        // Wait for at least on writer to connect.
        reader.waitForWriters();

        // Prints out the received samples.
        reader.onSamples(
            nullptr,
            [](const DataStorm::Sample<string, chrono::system_clock::time_point>& sample)
            {
                auto timeString = Time::formatTime(sample.getValue());
                cout << "received time for `" << sample.getKey() << "': " << timeString << endl;
            });

        // Exit once the user hits Ctrl-C to shutdown the node.
        node.waitForShutdown();
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
