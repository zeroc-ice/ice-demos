// Copyright (c) ZeroC, Inc.

#ifndef POLLING_CHAT_SESSION_I_H
#define POLLING_CHAT_SESSION_I_H

#include <ChatRoom.h>
#include <Ice/Ice.h>
#include <PollingChat.h>

class PollCallbackAdapter;

class PollingChatSessionI final : public PollingChat::PollingChatSession
{
public:
    PollingChatSessionI(
        const std::shared_ptr<ChatRoom>&,
        const std::string&,
        bool trace,
        const std::shared_ptr<Ice::Logger>& logger);

    void destroy(const Ice::Current&) final;
    Ice::StringSeq getInitialUsers(const Ice::Current&) final;
    PollingChat::ChatRoomEventSeq getUpdates(const Ice::Current&) final;
    int64_t send(std::string, const Ice::Current&) final;

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
