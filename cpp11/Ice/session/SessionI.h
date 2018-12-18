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
#include <mutex>

class SessionI : public Demo::Session
{
public:

    SessionI(const std::string&);

    virtual std::shared_ptr<Demo::HelloPrx> createHello(const Ice::Current&) override;
    virtual std::string getName(const Ice::Current&) override;
    virtual void destroy(const Ice::Current&) override;

private:

    std::mutex _mutex;
    const std::string _name;
    int _nextId; // The per-session id of the next hello object. This is used for tracing purposes.
    std::list<std::shared_ptr<Demo::HelloPrx>> _objs; // List of per-session allocated hello objects.
    bool _destroy;
};
#endif
