// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef SESSION_I_H
#define SESSION_I_H

#include <Ice/Ice.h>
#include <Session.h>
#include <list>

class SessionI : public Demo::Session
{
public:

    SessionI(const std::string&);

    virtual Demo::HelloPrx createHello(const Ice::Current&);
    virtual std::string getName(const Ice::Current&);
    virtual void destroy(const Ice::Current&);

private:

    IceUtil::Mutex _mutex;
    const std::string _name;
    int _nextId; // The per-session id of the next hello object. This is used for tracing purposes.
    std::list<Demo::HelloPrx> _objs; // List of per-session allocated hello objects.
    bool _destroy;
};
typedef IceUtil::Handle<SessionI> SessionIPtr;

#endif
