//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef SESSION_I_H
#define SESSION_I_H

#include "Session.h"
#include <Ice/Ice.h>
#include <list>
#include <mutex>

class SessionI final : public Demo::Session
{
public:
    SessionI(std::string);

    std::optional<Demo::HelloPrx> createHello(const Ice::Current&) final;
    std::string getName(const Ice::Current&) final;
    void destroy(const Ice::Current&) final;

private:
    std::mutex _mutex;
    const std::string _name;
    int _nextId; // The per-session id of the next hello object. This is used for tracing purposes.
    std::list<std::optional<Demo::HelloPrx>> _objs; // List of per-session allocated hello objects.
    bool _destroy;
};
#endif
