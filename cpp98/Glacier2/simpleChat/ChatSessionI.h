// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_SESSION_I_H
#define CHAT_SESSION_I_H

#include <Chat.h>

class ChatSessionI : public Demo::ChatSession
{
public:

    ChatSessionI(const std::string&);

    virtual void setCallback(const Demo::ChatCallbackPrx&, const Ice::Current&);
    virtual void say(const std::string&, const Ice::Current&);
    virtual void destroy(const Ice::Current&);

private:

    IceUtil::Mutex _mutex;
    const std::string _userId;
    Demo::ChatCallbackPrx _callback;
};

class ChatSessionManagerI : public Glacier2::SessionManager
{
public:

    virtual Glacier2::SessionPrx create(const std::string&, const Glacier2::SessionControlPrx&, const Ice::Current&);

    void destroy();
};

typedef IceUtil::Handle<ChatSessionManagerI> ChatSessionManagerIPtr;

#endif
