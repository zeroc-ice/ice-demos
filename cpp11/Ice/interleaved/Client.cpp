// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Throughput.h>

#include <iomanip>

#include <list>

using namespace std;
using namespace Demo;

class InterleavedClient : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    InterleavedClient app;
    return app.main(argc, argv, "config.client");
}

int
InterleavedClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto throughput = Ice::checkedCast<ThroughputPrx>(communicator()->propertyToProxy("Throughput.Proxy"));
    if(!throughput)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    ByteSeq byteSeq(ByteSeqSize);
    pair<const Ice::Byte*, const Ice::Byte*> byteArr = make_pair(byteSeq.data(), byteSeq.data() + byteSeq.size());

    throughput->ice_ping(); // Initial ping to setup the connection.

    // The maximum number of outstanding requests. -1 means
    // unlimited. If the number of oustanding requests is unlimited
    // and the server is slower than the client in processing the
    // requests, high memory consumption will result.
    const int maxOutstanding = 8;

    const int repetitions = 1000;

    cout << "sending and receiving " << repetitions << " byte sequences of size " << ByteSeqSize << "..." << endl;

    auto start = chrono::high_resolution_clock::now();

    list<future<ByteSeq>> results;
    for(int i = 0; i < repetitions; ++i)
    {
        results.push_back(throughput->echoByteSeqAsync(byteArr));

        // Remove any completed requests from the list
        auto p = results.begin();
        while(p != results.end())
        {
            if(p->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                // cout << "e" << flush;
                p = results.erase(p);
            }
            else
            {
                ++p;
            }
        }

        // This avoids too many outstanding requests. This is desirable if the server doesn't limit the
        // number of threads, or the server process requests slower than then client can send them.
        while(maxOutstanding != -1 && static_cast<int>(results.size()) > maxOutstanding)
        {
            // cout << "w" << flush;
            results.front().wait();
            results.pop_front();
        }
    }

    // Wait for all outstanding requests to complete
    //
    while(results.size() > 0)
    {
        // cout << "W" << flush;
        results.front().wait();
        results.pop_front();
    }

    auto duration = chrono::duration<float, chrono::milliseconds::period>(chrono::high_resolution_clock::now() - start);

    cout << "time for " << repetitions << " sequences: " << duration.count() << "ms" << endl;
    cout << "time per sequence: " << duration.count() / repetitions << "ms" << endl;

    double mbit = 2 * repetitions * ByteSeqSize * 1 * 8.0 / duration.count() / 1000;

    cout << "throughput: " << setprecision(5) << mbit << "Mbps" << endl;

    throughput->shutdown();

    return EXIT_SUCCESS;
}
