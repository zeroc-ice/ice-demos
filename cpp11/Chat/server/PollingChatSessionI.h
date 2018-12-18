// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef POLLING_CHAT_SESSION_I_H
#define POLLING_CHAT_SESSION_I_H

#include <Ice/Ice.h>
#include <PollingChat.h>
#include <ChatRoom.h>

class PollCallbackAdapter;

class PollingChatSessionI : public PollingChat::PollingChatSession
{
public:

    PollingChatSessionI(const std::shared_ptr<ChatRoom>&, const std::string&, bool trace,
                        const std::shared_ptr<Ice::Logger>& logger);

    virtual void destroy(const Ice::Current&) override;
    virtual Ice::StringSeq getInitialUsers(const Ice::Current&) override;
    virtual PollingChat::ChatRoomEventSeq getUpdates(const Ice::Current&) override;
    virtual long long send(std::string, const Ice::Current&) override;

private:

    const std::shared_ptr<ChatRoom> _chatRoom;
    const std::string _name;
    bool _destroy = false;
    const std::shared_ptr<PollCallbackAdapter> _callback;
    std::mutex _mutex;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
