// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include <Callback.h>

class CallbackSenderI : public Demo::CallbackSender
{
public:

    virtual void initiateCallback(std::shared_ptr<Demo::CallbackReceiverPrx>, const Ice::Current&) override;
    virtual void shutdown(const Ice::Current&) override;
};

#endif
