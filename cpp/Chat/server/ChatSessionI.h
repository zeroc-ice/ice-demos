//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CHAT_SESSION_I_H
#define CHAT_SESSION_I_H

#include "ChatRoom.h"
#include "ChatSession.h"

class ChatSessionI final : public Chat::ChatSession
{
public:
    ChatSessionI(const std::shared_ptr<ChatRoom>&, std::string, bool trace, const std::shared_ptr<Ice::Logger>& logger);

    void setCallback(std::optional<Chat::ChatRoomCallbackPrx>, const Ice::Current&) final;
    std::int64_t send(std::string, const Ice::Current&) final;
    void destroy(const Ice::Current&) final;

private:
    const std::shared_ptr<ChatRoom> _chatRoom;
    const std::string _name;
    std::shared_ptr<ChatRoomCallbackAdapter> _callback;
    bool _destroy = false;
    std::mutex _mutex;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
