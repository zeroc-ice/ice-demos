// Copyright (c) ZeroC, Inc. All rights reserved.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include "Stock.h"

#include <random>
#include <thread>

using namespace std;
using namespace Demo;

namespace
{

    std::random_device random;

    DataStorm::SingleKeyWriter<string, Stock>
    makeStock(DataStorm::Topic<string, Stock>& topic, string ticker, Stock stock)
    {
        // Create a stock writer for the given ticker and add the initial stock value.
        auto writer = DataStorm::makeSingleKeyWriter(topic, move(ticker));
        writer.add(move(stock));
        return writer;
    }

    void updateStock(DataStorm::SingleKeyWriter<string, Stock>& stock)
    {
        // Send a partial update to either update the price or the volume with the given writer.
        if (uniform_int_distribution<int>(1, 10)(random) < 8)
        {
            auto price = stock.getLast().getValue().price;
            stock.partialUpdate<float>("price")(uniform_real_distribution<float>(price * 0.95f, price * 1.05f)(random));
        }
        else
        {
            auto volume = stock.getLast().getValue().volume;
            stock.partialUpdate<int>("volume")(
                uniform_int_distribution<int>(volume * 95 / 100, volume * 105 / 100)(random));
        }
    }

}

int
main(int argc, char* argv[])
{
    try
    {
        // CtrlCHandler must be called before the node is created or any other threads are started.
        Ice::CtrlCHandler ctrlCHandler;

        // Instantiates node.
        DataStorm::Node node(argc, argv, "config.writer");

        // Shutdown the node on Ctrl-C.
        ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

        // Instantiates the "stock" topic.
        DataStorm::Topic<string, Stock> topic(node, "stocks");

        // Setup partial update updaters. The updater is responsible for updating the element value when a partial
        // update is received. Updaters must be set on both the topic reader and writer.
        topic.setUpdater<float>("price", [](Stock& stock, float price) { stock.price = price; });
        topic.setUpdater<int>("volume", [](Stock& stock, int volume) { stock.volume = volume; });

        // Setup a few stocks
        cout << "Available stocks: " << endl;
        map<string, Stock> stocks{
            {"GOOG", Stock("Google", 1040.61, 1035, 1043.178, 723018024728, 1035.96, 536996)},
            {"AAPL", Stock("Apple", 174.74, 174.44, 175.50, 898350570640, 174.96, 14026673)},
            {"FB", Stock("Facebook", 182.78, 180.29, 183.15, 531123722959, 180.87, 9426283)},
            {"AMZN", Stock("Amazon", 1186.41, 1160.70, 1186.84, 571697967142, 1156.16, 3442959)},
            {"MSFT", Stock("Microsoft", 83.27, 82.78, 83.43, 642393925538, 83.11, 6056186)}};

        for (const auto& stock : stocks)
        {
            cout << stock.first << endl;
        }

        string stock;
        cout << "Please enter the stock to publish (default = all):\n";
        getline(cin, stock);

        // Instantiate writers for the choosen stocks.
        vector<DataStorm::SingleKeyWriter<string, Stock>> writers;
        if (stock.empty() || stock == "all")
        {
            for (const auto& stock : stocks)
            {
                writers.push_back(makeStock(topic, stock.first, stock.second));
            }
        }
        else
        {
            if (stocks.find(stock) == stocks.end())
            {
                cout << "unknown stock `" << stock << "'" << endl;
                return 1;
            }
            writers.push_back(makeStock(topic, stock, stocks[stock]));
        }

        // Update the stock value or volume attributes.
        while (!node.isShutdown())
        {
            this_thread::sleep_for(chrono::seconds(1));
            for (auto& w : writers)
            {
                updateStock(w);
            }
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
