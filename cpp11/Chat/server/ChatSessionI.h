// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_SESSION_I_H
#define CHAT_SESSION_I_H

#include <ChatSession.h>
#include <ChatRoom.h>

class ChatSessionI : public Chat::ChatSession
{
public:

    ChatSessionI(const std::shared_ptr<ChatRoom>&, const std::string&, bool trace, const std::shared_ptr<Ice::Logger>& logger);

    virtual void setCallback(std::shared_ptr<Chat::ChatRoomCallbackPrx>, const Ice::Current&) override;
    virtual long long send(std::string, const Ice::Current&) override;
    virtual void destroy(const Ice::Current&) override;

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
