// Copyright (c) ZeroC, Inc.

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
        // CtrlCHandler must be called before the node is created or any other threads are started.
        Ice::CtrlCHandler ctrlCHandler;

        // Instantiates node.
        DataStorm::Node node(argc, argv, "config.reader");

        // Shutdown the node on Ctrl-C.
        ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

        // Instantiates the "time" topic.
        DataStorm::Topic<string, chrono::system_clock::time_point> topic(node, "time");

        // Instantiate a reader to read the time from all the topic cities.
        auto reader = DataStorm::makeAnyKeyReader(topic);

        // Wait for at least on writer to connect.
        reader.waitForWriters();

        // Prints out the received samples.
        reader.onSamples(
            nullptr,
            [](const DataStorm::Sample<string, chrono::system_clock::time_point>& sample)
            {
                auto time = chrono::system_clock::to_time_t(sample.getValue());
                std::tm timeInfo;

#ifdef _MSC_VER
                if (localtime_s(&timeInfo, &time))
                {
                    cout << "localtime_s error" << endl;
                    return;
                }
#else
                if (!localtime_r(&time, &timeInfo))
                {
                    cout << "localtime_r error" << endl;
                    return;
                }
#endif
                char timeString[100];
                if (!strftime(timeString, sizeof(timeString), "%x %X", &timeInfo))
                {
                    cout << "strftime error" << endl;
                    return;
                }

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
