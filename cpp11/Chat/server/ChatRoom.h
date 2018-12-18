// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_ROOM_I_H
#define CHAT_ROOM_I_H

#include <Ice/Ice.h>
#include <PollingChat.h>
#include <set>

class ChatRoomCallbackAdapter
{
public:

    virtual ~ChatRoomCallbackAdapter() = default;

    virtual void init(Ice::StringSeq) = 0;
    virtual void join(const std::shared_ptr<PollingChat::UserJoinedEvent>&) = 0;
    virtual void leave(const std::shared_ptr<PollingChat::UserLeftEvent>&) = 0;
    virtual void send(const std::shared_ptr<PollingChat::MessageEvent>&) = 0;
};

class ChatRoom
{
public:

    ChatRoom(bool trace, const std::shared_ptr<Ice::Logger>& logger);
    void reserve(const std::string&);
    void unreserve(const std::string&);
    void join(const std::string&, const std::shared_ptr<ChatRoomCallbackAdapter>&);
    void leave(const std::string&);
    long long send(const std::string&, std::string);

private:

    using ChatRoomCallbackMap = std::map<std::string, std::shared_ptr<ChatRoomCallbackAdapter>>;

    ChatRoomCallbackMap _members;
    std::set<std::string> _reserved;
    std::mutex _mutex;
    const bool _trace;
    const std::shared_ptr<Ice::Logger> _logger;
};

#endif
