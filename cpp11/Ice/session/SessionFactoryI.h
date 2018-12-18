// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef SESSION_FACTORY_I_H
#define SESSION_FACTORY_I_H

#include <Ice/Ice.h>
#include <Session.h>

class SessionFactoryI : public Demo::SessionFactory
{
public:

    virtual std::shared_ptr<Demo::SessionPrx> create(std::string, const Ice::Current&) override;
    virtual void shutdown(const Ice::Current&) override;
};

#endif
