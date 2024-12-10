// Copyright (c) ZeroC, Inc. All rights reserved.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include "Stock.h"

#include <iostream>

using namespace std;
using namespace Demo;

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

        // Instantiates the "stocks" topic.
        DataStorm::Topic<string, Demo::Stock> topic(node, "stocks");

        // Setup partial update updaters. The updater is responsible for updating the element value when a partial
        // update is received. Updaters must be set on the topic from both the reader and writer.
        topic.setUpdater<float>("price", [](Stock& stock, float price) { stock.price = price; });
        topic.setUpdater<int>("volume", [](Stock& stock, int volume) { stock.volume = volume; });

        shared_ptr<DataStorm::Topic<string, Demo::Stock>::ReaderType> reader;
        {
            // Create a reader that connects to all the keys but doesn't receive any samples (we use the `_event'
            // predefined sample filter with an empty set of sample events to discard events on the writer).
            auto stocks = DataStorm::makeAnyKeyReader(
                topic,
                DataStorm::Filter<DataStorm::SampleEventSeq>("_event", DataStorm::SampleEventSeq{}));
            stocks.waitForWriters();

            // Get the set of stocks connected with the any reader and display their ticker.
            std::promise<void> p;
            stocks.onConnectedKeys(
                [&p](vector<string> tickers)
                {
                    cout << "Available stock(s): " << endl;
                    for (auto ticker : tickers)
                    {
                        cout << ticker << endl;
                    }
                    p.set_value();
                },
                [](DataStorm::CallbackReason action, string ticker)
                {
                    if (action == DataStorm::CallbackReason::Connect)
                    {
                        cout << "New stock available: " << ticker << endl;
                    }
                    else
                    {
                        cout << "Stock no longer available: " << ticker << endl;
                    }
                });
            p.get_future().get();

            string stock;
            cout << "Please enter the stock to follow (default = all):\n";
            getline(cin, stock);

            // Read values for the given stock using a key reader.
            if (stock.empty() || stock == "all")
            {
                reader = make_shared<DataStorm::MultiKeyReader<string, Stock>>(topic, vector<string>{});
            }
            else
            {
                auto tickers = stocks.getConnectedKeys();
                if (find(tickers.begin(), tickers.end(), stock) == tickers.end())
                {
                    cout << "unknown stock `" << stock << "'" << endl;
                    return 1;
                }
                reader = make_shared<DataStorm::SingleKeyReader<string, Stock>>(topic, stock);
            }
        }

        // Wait for the writer to be connected.
        reader->waitForWriters();

        auto displaySample = [](DataStorm::Sample<string, Stock> s)
        {
            if (s.getEvent() == DataStorm::SampleEvent::Add || s.getEvent() == DataStorm::SampleEvent::Update)
            {
                auto value = s.getValue();
                cout << "Stock: " << value.name << " (" << s.getKey() << ")" << endl;
                cout << "Price: " << value.price << endl;
                cout << "Best bid/ask: " << value.bestBid << '/' << value.bestAsk << endl;
                cout << "Market Cap: " << value.marketCap << endl;
                cout << "Previous close: " << value.previousClose << endl;
                cout << "Volume: " << value.volume << endl;
                cout << endl;
            }
            else if (s.getEvent() == DataStorm::SampleEvent::PartialUpdate)
            {
                if (s.getUpdateTag() == "price")
                {
                    cout << "received price update for " << s.getKey() << ": " << s.getValue().price << endl;
                }
                else if (s.getUpdateTag() == "volume")
                {
                    cout << "received volume update for " << s.getKey() << ": " << s.getValue().volume << endl;
                }
            }
        };

        // Print out the sample values queued with this reader.
        reader->onSamples(
            [displaySample](const vector<DataStorm::Sample<string, Stock>>& samples)
            {
                for (auto& s : samples)
                {
                    displaySample(s);
                }
            },
            displaySample);

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
