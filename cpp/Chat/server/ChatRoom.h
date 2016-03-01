// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_ROOM_I_H
#define CHAT_ROOM_I_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <Ice/BuiltinSequences.h>

#include <string>
#include <set>

#include <PollingChat.h>

class ChatRoomCallbackAdapter : public IceUtil::Shared
{
public:

    virtual void init(const Ice::StringSeq&) = 0;
    virtual void join(const PollingChat::UserJoinedEventPtr&) = 0;
    virtual void leave(const PollingChat::UserLeftEventPtr&) = 0;
    virtual void send(const PollingChat::MessageEventPtr&) = 0;
};
typedef IceUtil::Handle<ChatRoomCallbackAdapter> ChatRoomCallbackAdapterPtr;

class ChatRoom : public IceUtil::Shared
{
public:

    ChatRoom(bool trace, const Ice::LoggerPtr& logger);
    void reserve(const std::string&);
    void unreserve(const std::string&);
    void join(const std::string&, const ChatRoomCallbackAdapterPtr&);
    void leave(const std::string&);
    Ice::Long send(const std::string&, const std::string&);

private:

    typedef std::map<std::string, ChatRoomCallbackAdapterPtr> ChatRoomCallbackMap;

    ChatRoomCallbackMap _members;
    std::set<std::string> _reserved;
    IceUtil::Mutex _mutex;
    const bool _trace;
    const Ice::LoggerPtr _logger;
};
typedef IceUtil::Handle<ChatRoom> ChatRoomPtr;

#endif
