// Copyright (c) ZeroC, Inc.

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include "Callback.h"

class CallbackSenderI final : public Demo::CallbackSender
{
public:
    void initiateCallback(std::optional<Demo::CallbackReceiverPrx>, const Ice::Current&) final;
    void shutdown(const Ice::Current&) final;
};

#endif
