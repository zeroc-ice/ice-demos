// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_SESSION_MANAGER_I_H
#define CHAT_SESSION_MANAGER_I_H

#include <Glacier2/Glacier2.h>
#include <string>
#include <ChatRoom.h>

class ChatSessionManagerI : public Glacier2::SessionManager
{
public:

    ChatSessionManagerI(const std::shared_ptr<ChatRoom>&, bool trace, const std::shared_ptr<Ice::Logger>& logger);

    virtual std::shared_ptr<Glacier2::SessionPrx> create(std::string,
                                                         std::shared_ptr<Glacier2::SessionControlPrx>,
                                                         const Ice::Current&) override;
private:

    const std::shared_ptr<ChatRoom> _chatRoom;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
