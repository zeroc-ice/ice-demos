// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include <Callback.h>

class CallbackSenderI : public Demo::CallbackSender,
                        public std::enable_shared_from_this<CallbackSenderI>
{
public:

    virtual void addClient(std::shared_ptr<Demo::CallbackReceiverPrx>, const Ice::Current&) override;

    void destroy();
    void start();

private:

    void invokeCallback();
    void removeClient(const std::shared_ptr<Demo::CallbackReceiverPrx>&, std::exception_ptr);

    bool _destroy = false;
    std::vector<std::shared_ptr<Demo::CallbackReceiverPrx>> _clients;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::future<void> _result;
};

#endif
