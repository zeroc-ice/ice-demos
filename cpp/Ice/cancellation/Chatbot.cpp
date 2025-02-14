// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <future>
#include <iostream>
#include <sstream>

using namespace std;

Server::Chatbot::Chatbot(std::chrono::milliseconds delay, std::shared_future<void> cancelDispatch)
    : _cancelDispatch{std::move(cancelDispatch)}, _delay{delay}
{
}

string
Server::Chatbot::greet(string name, const Ice::Current&)
{
    cout << "Dispatching greet request { name = '" << name << "' }" << endl;

    if (_delay > std::chrono::milliseconds::zero())
    {
        // Each thread needs its own copy of the shared_future.
        shared_future<void> cancelDispatch{_cancelDispatch};

        // Wait for delay or until _cancelDispatch is completed.
        // This call blocks a dispatch thread from the Ice server thread pool, which means we need to configure the
        // Ice server thread pool to have enough threads.
        if (cancelDispatch.wait_for(_delay) == std::future_status::ready)
        {
            cout << "greet dispatch canceled { name = '" << name << "' }" << endl;
            throw Ice::UnknownException{__FILE__, __LINE__, "greet dispatch canceled"};
        }
    }

    ostringstream os;
    os << "Hello, " << name << "!";
    return os.str();
}
