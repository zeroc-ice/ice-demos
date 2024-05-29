//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include "Callback.h"

class CallbackI final : public Demo::Callback
{
public:
    void initiateCallback(std::optional<Demo::CallbackReceiverPrx>, const Ice::Current&) final;
    void shutdown(const Ice::Current&) final;
};

#endif
