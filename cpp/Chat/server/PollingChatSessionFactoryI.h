//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef POLLING_CHAT_SESSION_FACTORY_I_H
#define POLLING_CHAT_SESSION_FACTORY_I_H

#include "ChatRoom.h"
#include "PollingChat.h"
#include "PollingChatSessionI.h"
#include <Ice/Ice.h>

class PollingChatSessionFactoryI final : public PollingChat::PollingChatSessionFactory
{
public:
    PollingChatSessionFactoryI(const std::shared_ptr<ChatRoom>&, bool, const std::shared_ptr<Ice::Logger>&);

    std::optional<PollingChat::PollingChatSessionPrx> create(std::string, std::string, const Ice::Current&) final;

private:
    const std::shared_ptr<ChatRoom> _chatRoom;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
