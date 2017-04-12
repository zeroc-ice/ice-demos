// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include <Callback.h>
#include <set>

class CallbackSenderI : public Demo::CallbackSender
{
public:

    CallbackSenderI(std::shared_ptr<Ice::Communicator>);

    void destroy();
    void start();

    virtual void addClient(Ice::Identity, const Ice::Current&) override;

private:

    void invokeCallback();
    void removeClient(const std::shared_ptr<Demo::CallbackReceiverPrx>&, std::exception_ptr);

    std::shared_ptr<Ice::Communicator> _communicator;
    bool _destroy = false;
    std::vector<std::shared_ptr<Demo::CallbackReceiverPrx>> _clients;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::future<void> _result;
};

#endif
