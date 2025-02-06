// Copyright (c) ZeroC, Inc.

#ifndef CHAT_SESSION_MANAGER_I_H
#define CHAT_SESSION_MANAGER_I_H

#include "ChatRoom.h"

#include <Glacier2/Glacier2.h>
#include <string>

class ChatSessionManagerI final : public Glacier2::SessionManager
{
public:
    ChatSessionManagerI(const std::shared_ptr<ChatRoom>&, bool trace, const std::shared_ptr<Ice::Logger>& logger);

    std::optional<Glacier2::SessionPrx>
    create(std::string, std::optional<Glacier2::SessionControlPrx>, const Ice::Current&) final;

private:
    const std::shared_ptr<ChatRoom> _chatRoom;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
