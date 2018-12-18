// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_SESSION_I_H
#define CHAT_SESSION_I_H

#include <Chat.h>
#include <Glacier2/Glacier2.h>
#include <mutex>

class ChatSessionI : public Demo::ChatSession
{
public:

    ChatSessionI(const std::string&);

    virtual void setCallback(std::shared_ptr<Demo::ChatCallbackPrx>, const Ice::Current&) override;
    virtual void say(std::string, const Ice::Current&) override;
    virtual void destroy(const Ice::Current&) override;

private:

    std::mutex _mutex;
    const std::string _userId;
    std::shared_ptr<Demo::ChatCallbackPrx> _callback;
};

class ChatSessionManagerI : public Glacier2::SessionManager
{
public:

    virtual std::shared_ptr<Glacier2::SessionPrx>
    create(std::string, std::shared_ptr<Glacier2::SessionControlPrx>, const Ice::Current&) override;

    void destroy();
};

#endif
