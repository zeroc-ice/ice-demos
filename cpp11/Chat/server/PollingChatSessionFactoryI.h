// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef POLLING_CHAT_SESSION_FACTORY_I_H
#define POLLING_CHAT_SESSION_FACTORY_I_H

#include <Ice/Ice.h>

#include <PollingChat.h>
#include <PollingChatSessionI.h>
#include <ChatRoom.h>

class PollingChatSessionFactoryI : public PollingChat::PollingChatSessionFactory
{
public:

    PollingChatSessionFactoryI(const std::shared_ptr<ChatRoom>&, int, bool, const std::shared_ptr<Ice::Logger>&);

    virtual std::shared_ptr<PollingChat::PollingChatSessionPrx>
    create(std::string, std::string, const Ice::Current&) override;

private:

    const std::shared_ptr<ChatRoom> _chatRoom;
    const int _timeout;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
