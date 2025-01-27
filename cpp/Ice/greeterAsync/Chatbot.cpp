// Copyright (c) ZeroC, Inc.

#include "Chatbot.h"

#include <chrono>
#include <future>
#include <iostream>
#include <sstream>

using namespace std;

GreeterServer::Chatbot::~Chatbot()
{
    // Wait for all tasks to complete.
    for (auto& task : _tasks)
    {
        task.wait();
    }
}

void
GreeterServer::Chatbot::greetAsync(
    string name,
    function<void(string_view)> response,
    [[maybe_unused]] function<void(std::exception_ptr)> exception,
    const Ice::Current&)
{
    cout << "Dispatching greet request { name = " << name << " } asynchronously" << endl;

    // Simulate a long-running operation by using std::async.
    // Note that we're moving all arguments except current into the lambda expression.
    _tasks.push_back(std::async(
        std::launch::async,
        [name = std::move(name), response = std::move(response), exception = std::move(exception)]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            ostringstream os;
            os << "Hello, " << name << "!";
            response(os.str());
        }));

    // greetAsync completes immediately (releases the Ice server thread pool) while the task runs in the background.
}
