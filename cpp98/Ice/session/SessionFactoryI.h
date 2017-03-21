// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
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

private:

    IceUtil::Mutex _mutex;
    std::map<Ice::ConnectionPtr, Demo::SessionPrx> _connectionMap;
};

typedef IceUtil::Handle<SessionFactoryI> SessionFactoryIPtr;

#endif
