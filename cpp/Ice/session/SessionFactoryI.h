// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef SESSION_FACTORY_I_H
#define SESSION_FACTORY_I_H

#include <Ice/Ice.h>
#include <Session.h>
#include <ReapTask.h>

class SessionFactoryI : public Demo::SessionFactory
{
public:

    SessionFactoryI(const ReapTaskPtr&);

    virtual Demo::SessionPrx create(const std::string&, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);


    SessionFactoryI& operator=(const SessionFactoryI&) { return *this; }

private:

    const ReapTaskPtr _reaper;
};

#endif
