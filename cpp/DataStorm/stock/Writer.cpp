// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include "Stock.h"

#include <iostream>
#include <random>
#include <thread>

using namespace std;
using namespace Demo;

namespace
{
    DataStorm::SingleKeyWriter<string, Stock>
    makeStock(DataStorm::Topic<string, Stock>& topic, const string& ticker, const Stock& stock)
    {
        // Create a stock writer for the given ticker and add the initial stock value.
        auto writer = DataStorm::makeSingleKeyWriter(topic, ticker);
        writer.add(stock);
        return writer;
    }

    void updateStock(DataStorm::SingleKeyWriter<string, Stock>& stock)
    {
        static std::random_device random;

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
        // Set a custom terminate handler to print unhandled exceptions to cerr.
        std::set_terminate(Terminate::printCurrentException);

        // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the
        // beginning of the program, before creating a DataStorm node or starting any thread.
        Ice::CtrlCHandler ctrlCHandler;

        // Instantiates node.
        DataStorm::Node node{argc, argv};

        // Shutdown the node on Ctrl-C.
        ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

        // Instantiates the "stock" topic.
        DataStorm::Topic<string, Stock> topic{node, "stocks"};

        // Setup partial update updaters. The updater is responsible for updating the element value when a partial
        // update is received. Updaters must be set on both the topic reader and writer.
        topic.setUpdater<float>("price", [](Stock& stock, float price) { stock.price = price; });
        topic.setUpdater<int>("volume", [](Stock& stock, int volume) { stock.volume = volume; });

        // Setup a few stocks
        cout << "Available stocks: " << endl;
        map<string, Stock> stocks{
            {"GOOG",
             Stock{
                 .name = "Google",
                 .price = 1040.61f,
                 .bestBid = 1035.0f,
                 .bestAsk = 1043.178f,
                 .marketCap = 723018024728,
                 .previousClose = 1035.96f,
                 .volume = 536996}},
            {"AAPL",
             Stock{
                 .name = "Apple",
                 .price = 174.74f,
                 .bestBid = 174.44f,
                 .bestAsk = 175.50f,
                 .marketCap = 898350570640,
                 .previousClose = 174.96f,
                 .volume = 14026673}},
            {"FB",
             Stock{
                 .name = "Facebook",
                 .price = 182.78f,
                 .bestBid = 180.29f,
                 .bestAsk = 183.15f,
                 .marketCap = 531123722959,
                 .previousClose = 180.87f,
                 .volume = 9426283}},
            {"AMZN",
             Stock{
                 .name = "Amazon",
                 .price = 1186.41f,
                 .bestBid = 1160.70f,
                 .bestAsk = 1186.84f,
                 .marketCap = 571697967142,
                 .previousClose = 1156.16f,
                 .volume = 3442959}},
            {"MSFT",
             Stock{
                 .name = "Microsoft",
                 .price = 83.27f,
                 .bestBid = 82.78f,
                 .bestAsk = 83.43f,
                 .marketCap = 642393925538,
                 .previousClose = 83.11f,
                 .volume = 6056186}}};

        for (const auto& stock : stocks)
        {
            cout << stock.first << endl;
        }

        string stockName;
        cout << "Please enter the stock to publish (default = all):\n";
        getline(cin, stockName);

        // Instantiate writers for the choosen stocks.
        vector<DataStorm::SingleKeyWriter<string, Stock>> writers;
        if (stockName.empty() || stockName == "all")
        {
            for (const auto& stock : stocks)
            {
                writers.push_back(makeStock(topic, stock.first, stock.second));
            }
        }
        else
        {
            if (stocks.find(stockName) == stocks.end())
            {
                cout << "unknown stock `" << stockName << "'" << endl;
                return 1;
            }
            writers.push_back(makeStock(topic, stockName, stocks[stockName]));
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
