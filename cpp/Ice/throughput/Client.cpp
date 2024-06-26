//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Throughput.h"
#include <Ice/Ice.h>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceWS();
#endif

    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch (const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

void menu();

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    auto throughput = Ice::checkedCast<ThroughputPrx>(communicator->propertyToProxy("Throughput.Proxy"));
    if (!throughput)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    auto throughputOneway = throughput->ice_oneway();

    ByteSeq byteSeq(ByteSeqSize);
    auto byteArr = make_pair(byteSeq.data(), byteSeq.data() + byteSeq.size());

    StringSeq stringSeq(StringSeqSize, "hello");
    const vector<std::string> stringViewSeq(StringSeqSize, "hello");

    StringDoubleSeq structSeq(StringDoubleSeqSize, {"hello", 3.14});
    const FixedSeq fixedSeq(FixedSeqSize, {0, 0, 0.0});

    //
    // To allow cross-language tests we may need to "warm up" the
    // server. The warm up is to ensure that any JIT compiler will
    // have converted any hotspots to native code. This ensures an
    // accurate throughput measurement.
    //
    if (throughput->needsWarmup())
    {
        throughput->startWarmup();

        ByteSeq warmupBytesBuf(1);
        auto warmupBytes = make_pair(warmupBytesBuf.data(), warmupBytesBuf.data() + warmupBytesBuf.size());

        const vector<std::string> warmupStringViews(1);
        const StringDoubleSeq warmupStructs(1);
        const FixedSeq warmupFixed(1);

        cout << "warming up the server... " << flush;
        for (int i = 0; i < 10000; i++)
        {
            throughput->sendByteSeq(warmupBytes);
            throughput->sendStringSeq(warmupStringViews);
            throughput->sendStructSeq(warmupStructs);
            throughput->sendFixedSeq(warmupFixed);

            throughput->recvByteSeq();
            throughput->recvStringSeq();
            throughput->recvStructSeq();
            throughput->recvFixedSeq();

            throughput->echoByteSeq(warmupBytes);
            throughput->echoStringSeq(warmupStringViews);
            throughput->echoStructSeq(warmupStructs);
            throughput->echoFixedSeq(warmupFixed);
        }

        throughput->endWarmup();

        cout << " ok" << endl;
    }
    else
    {
        throughput->ice_ping(); // Initial ping to setup the connection.
    }

    menu();

    //
    // By default use byte sequence.
    //
    char currentType = '1';
    int seqSize = ByteSeqSize;

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;

            auto start = chrono::high_resolution_clock::now();
            int repetitions = 100;

            if (c == '1' || c == '2' || c == '3' || c == '4')
            {
                currentType = c;
                switch (c)
                {
                    case '1':
                    {
                        cout << "using byte sequences" << endl;
                        seqSize = ByteSeqSize;
                        break;
                    }

                    case '2':
                    {
                        cout << "using string sequences" << endl;
                        seqSize = StringSeqSize;
                        break;
                    }

                    case '3':
                    {
                        cout << "using variable-length struct sequences" << endl;
                        seqSize = StringDoubleSeqSize;
                        break;
                    }

                    case '4':
                    {
                        cout << "using fixed-length struct sequences" << endl;
                        seqSize = FixedSeqSize;
                        break;
                    }
                }
            }
            else if (c == 't' || c == 'o' || c == 'r' || c == 'e')
            {
                if (currentType == '1')
                {
                    repetitions = 1000; // Use more iterations for byte sequences as it's a lot faster
                }

                switch (c)
                {
                    case 't':
                    case 'o':
                    {
                        cout << "sending";
                        break;
                    }

                    case 'r':
                    {
                        cout << "receiving";
                        break;
                    }

                    case 'e':
                    {
                        cout << "sending and receiving";
                        break;
                    }
                }

                cout << ' ' << repetitions;
                switch (currentType)
                {
                    case '1':
                    {
                        cout << " byte";
                        break;
                    }

                    case '2':
                    {
                        cout << " string";
                        break;
                    }

                    case '3':
                    {
                        cout << " variable-length struct";
                        break;
                    }

                    case '4':
                    {
                        cout << " fixed-length struct";
                        break;
                    }
                }
                cout << " sequences of size " << seqSize;

                if (c == 'o')
                {
                    cout << " as oneway";
                }

                cout << "..." << endl;

                for (int i = 0; i < repetitions; ++i)
                {
                    switch (currentType)
                    {
                        case '1':
                        {
                            switch (c)
                            {
                                case 't':
                                {
                                    throughput->sendByteSeq(byteArr);
                                    break;
                                }

                                case 'o':
                                {
                                    throughputOneway->sendByteSeq(byteArr);
                                    break;
                                }

                                case 'r':
                                {
                                    throughput->recvByteSeq();
                                    break;
                                }

                                case 'e':
                                {
                                    throughput->echoByteSeq(byteArr);
                                    break;
                                }
                            }
                            break;
                        }

                        case '2':
                        {
                            switch (c)
                            {
                                case 't':
                                {
                                    throughput->sendStringSeq(stringViewSeq);
                                    break;
                                }

                                case 'o':
                                {
                                    throughputOneway->sendStringSeq(stringViewSeq);
                                    break;
                                }

                                case 'r':
                                {
                                    throughput->recvStringSeq();
                                    break;
                                }

                                case 'e':
                                {
                                    throughput->echoStringSeq(stringViewSeq);
                                    break;
                                }
                            }
                            break;
                        }

                        case '3':
                        {
                            switch (c)
                            {
                                case 't':
                                {
                                    throughput->sendStructSeq(structSeq);
                                    break;
                                }

                                case 'o':
                                {
                                    throughputOneway->sendStructSeq(structSeq);
                                    break;
                                }

                                case 'r':
                                {
                                    throughput->recvStructSeq();
                                    break;
                                }

                                case 'e':
                                {
                                    throughput->echoStructSeq(structSeq);
                                    break;
                                }
                            }
                            break;
                        }

                        case '4':
                        {
                            switch (c)
                            {
                                case 't':
                                {
                                    throughput->sendFixedSeq(fixedSeq);
                                    break;
                                }

                                case 'o':
                                {
                                    throughputOneway->sendFixedSeq(fixedSeq);
                                    break;
                                }

                                case 'r':
                                {
                                    throughput->recvFixedSeq();
                                    break;
                                }

                                case 'e':
                                {
                                    throughput->echoFixedSeq(fixedSeq);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }

                auto duration =
                    chrono::duration<float, chrono::milliseconds::period>(chrono::high_resolution_clock::now() - start);
                cout << "time for " << repetitions << " sequences: " << duration.count() << "ms" << endl;
                cout << "time per sequence: " << duration.count() / static_cast<float>(repetitions) << "ms" << endl;
                int wireSize = 0;
                switch (currentType)
                {
                    case '1':
                    {
                        wireSize = 1;
                        break;
                    }
                    case '2':
                    {
                        wireSize = static_cast<int>(stringSeq[0].size());
                        break;
                    }
                    case '3':
                    {
                        wireSize = static_cast<int>(structSeq[0].s.size());
                        wireSize += 8; // Size of double on the wire.
                        break;
                    }
                    case '4':
                    {
                        wireSize = 16; // Size of two ints and a double on the wire.
                        break;
                    }
                }
                double mbit = repetitions * seqSize * wireSize * 8.0 / duration.count() / 1000;
                if (c == 'e')
                {
                    mbit *= 2;
                }
                cout << "throughput: " << setprecision(5) << mbit << "Mbps" << endl;
            }
            else if (c == 's')
            {
                throughput->shutdown();
            }
            else if (c == 'x')
            {
                // Nothing to do
            }
            else if (c == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch (const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    } while (cin.good() && c != 'x');

    return 0;
}

void
menu()
{
    cout << "usage:\n"
            "\n"
            "toggle type of data to send:\n"
            "1: sequence of bytes (default)\n"
            "2: sequence of strings (\"hello\")\n"
            "3: sequence of structs with a string (\"hello\") and a double\n"
            "4: sequence of structs with two ints and a double\n"
            "\n"
            "select test to run:\n"
            "t: Send sequence as twoway\n"
            "o: Send sequence as oneway\n"
            "r: Receive sequence\n"
            "e: Echo (send and receive) sequence\n"
            "\n"
            "other commands:\n"
            "s: shutdown server\n"
            "x: exit\n"
            "?: help\n";
}
