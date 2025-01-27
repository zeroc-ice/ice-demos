// Copyright (c) ZeroC, Inc.

#ifndef SESSION_FACTORY_I_H
#define SESSION_FACTORY_I_H

#include "Session.h"
#include <Ice/Ice.h>

class SessionFactoryI final : public Demo::SessionFactory
{
public:
    std::optional<Demo::SessionPrx> create(std::string, const Ice::Current&) final;
    void shutdown(const Ice::Current&) final;
};

#endif
