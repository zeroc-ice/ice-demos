// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

    virtual void addClient(Ice::Identity, const Ice::Current&);

private:

    std::shared_ptr<Ice::Communicator> _communicator;
    bool _destroy;
    std::set<std::shared_ptr<Demo::CallbackReceiverPrx>> _clients;
    
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _senderThread;
};

#endif
