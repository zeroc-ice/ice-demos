// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include <Callback.h>

class CallbackReceiverI : public Demo::CallbackReceiver
{
public:

    virtual void callback(const Ice::Current&);
};

class CallbackI : public Demo::Callback
{
public:

    virtual void initiateCallback(const Demo::CallbackReceiverPrx&, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

};

#endif
