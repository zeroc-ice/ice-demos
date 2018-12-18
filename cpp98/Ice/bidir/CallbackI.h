// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include <Ice/Ice.h>
#include <Callback.h>
#include <set>

class CallbackSenderI;
typedef IceUtil::Handle<CallbackSenderI> CallbackSenderIPtr;

class CallbackSenderI : public Demo::CallbackSender, public IceUtil::Thread, public IceUtil::Monitor<IceUtil::Mutex>
{
public:

    CallbackSenderI();

    void destroy();

    virtual void addClient(const Demo::CallbackReceiverPrx&, const Ice::Current&);

    virtual void run();

private:

    bool _destroy;
    std::set<Demo::CallbackReceiverPrx> _clients;
};

#endif
