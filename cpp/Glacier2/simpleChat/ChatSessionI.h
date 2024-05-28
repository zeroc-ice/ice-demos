//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CHAT_SESSION_I_H
#define CHAT_SESSION_I_H

#include <Chat.h>
#include <Glacier2/Glacier2.h>
#include <mutex>

class ChatSessionI final : public Demo::ChatSession
{
public:
    ChatSessionI(std::string);

    void setCallback(std::optional<Demo::ChatCallbackPrx>, const Ice::Current&) final;
    void say(std::string, const Ice::Current&) final;
    void destroy(const Ice::Current&) final;

private:
    std::mutex _mutex;
    const std::string _userId;
    std::optional<Demo::ChatCallbackPrx> _callback;
};

class ChatSessionManagerI final : public Glacier2::SessionManager
{
public:
    std::optional<Glacier2::SessionPrx>
    create(std::string, std::optional<Glacier2::SessionControlPrx>, const Ice::Current&) final;

    void destroy();
};

#endif
