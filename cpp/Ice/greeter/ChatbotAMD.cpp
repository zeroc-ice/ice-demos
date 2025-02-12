// Copyright (c) ZeroC, Inc.

#include "ChatbotAMD.h"

#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <sstream>

using namespace std;

ServerAMD::Chatbot::~Chatbot()
{
    // Wait for all tasks to complete.
    for (auto& task : _tasks)
    {
        task.wait();
    }
}

void
ServerAMD::Chatbot::greetAsync(
    string name,
    function<void(string_view)> response,
    [[maybe_unused]] function<void(std::exception_ptr)> exception,
    const Ice::Current&)
{
    // This function executes in an Ice server thread pool thread when the server receives a request. The Ice server
    // thread pool is configured with a single thread in this demo (the default configuration); as a result, we don't
    // need to serialize access to the _tasks field.

    cout << "Dispatching greet request { name = '" << name << "' }" << endl;

    // Simulate a long-running background operation by using std::async.
    // Note that we're moving all arguments except current into the lambda expression.

    _tasks.push_back(std::async(
        std::launch::async,
        [name = std::move(name), response = std::move(response), exception = std::move(exception)]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            ostringstream os;
            os << "Hello, " << name << "!";
            response(os.str());
        }));

    // We don't want the _tasks vector to grow forever so remove all completed task here, without waiting.
    // TODO: switch to std::erase_if when we can use C++20.
    _tasks.erase(
        std::remove_if(
            _tasks.begin(),
            _tasks.end(),
            [](const auto& task) { return task.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }),
        _tasks.end());

    // greetAsync completes immediately (releasing the Ice server thread pool thread) while the task runs in the
    // background.
}
