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

    virtual Demo::SessionPrx create(const std::string&, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

    void deadClient(const Ice::ConnectionPtr&);
};

typedef IceUtil::Handle<SessionFactoryI> SessionFactoryIPtr;

#endif
