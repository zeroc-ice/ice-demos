// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>
#include <string>
#include <thread>

using namespace std;

namespace DataStorm
{
    template<> struct Encoder<chrono::system_clock::time_point>
    {
        static Ice::ByteSeq encode(const chrono::system_clock::time_point& time)
        {
            // Encode the number of seconds since epoch. The value is encoded in a way which doesn't depend on the
            // platform endianess (little endian with variable number of bytes).
            Ice::ByteSeq data;
            auto value = chrono::time_point_cast<chrono::seconds>(time).time_since_epoch().count();
            while (value)
            {
                data.push_back(static_cast<std::byte>(value % 256));
                value = value / 256;
            }
            return data;
        }
    };

    template<> struct Decoder<chrono::system_clock::time_point>
    {
        static chrono::system_clock::time_point decode(const Ice::ByteSeq&)
        {
            assert(false); // Not used by the reader but it still needs to be declared.
            return {};
        }
    };
};

int
main(int argc, char* argv[])
{
    try
    {
        // CtrlCHandler must be called before the node is created or any other threads are started.
        Ice::CtrlCHandler ctrlCHandler;

        // Instantiates node.
        DataStorm::Node node{argc, argv};

        // Shutdown the node on Ctrl-C.
        ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

        // Asks for city name to publish updates
        string city;
        cout << "Please enter city name: ";
        getline(cin, city);

        // Instantiates the "time" topic.
        DataStorm::Topic<string, chrono::system_clock::time_point> topic{node, "time"};

        // Instantiate a writer to write the time from the given city.
        auto writer = DataStorm::makeSingleKeyWriter(topic, city);

        while (!node.isShutdown())
        {
            writer.update(chrono::system_clock::now());
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
