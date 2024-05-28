//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include "Callback.h"

class CallbackSenderI final : public Demo::CallbackSender, public std::enable_shared_from_this<CallbackSenderI>
{
public:
    void addClient(std::optional<Demo::CallbackReceiverPrx>, const Ice::Current&) final;

    void destroy();
    void start();

private:
    void invokeCallback();
    void removeClient(const std::optional<Demo::CallbackReceiverPrx>&, std::exception_ptr);

    bool _destroy = false;
    std::vector<std::optional<Demo::CallbackReceiverPrx>> _clients;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::future<void> _result;
};

#endif
