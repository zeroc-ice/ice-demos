// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef POLLING_CHAT_SESSION_I_H
#define POLLING_CHAT_SESSION_I_H

#include <Ice/Ice.h>

#include <PollingChat.h>
#include <ChatRoom.h>

#include <list>

class PollCallbackAdapter;
typedef IceUtil::Handle<PollCallbackAdapter> PollCallbackAdapterPtr;

class PollingChatSessionI : public PollingChat::PollingChatSession
{
public:

    PollingChatSessionI(const ChatRoomPtr&, const std::string&, bool trace, const Ice::LoggerPtr& logger);

    virtual void destroy(const Ice::Current&);
    virtual Ice::StringSeq getInitialUsers(const Ice::Current&);
    virtual PollingChat::ChatRoomEventSeq getUpdates(const Ice::Current&);
    virtual Ice::Long send(const std::string&, const Ice::Current&);

    virtual IceUtil::Time timestamp() const;

private:

    const ChatRoomPtr _chatRoom;
    const std::string _name;
    IceUtil::Time _timestamp; // The last time the session was refreshed.
    bool _destroy;
    const PollCallbackAdapterPtr _callback;
    IceUtil::Mutex _mutex;
    const bool _trace;
    const Ice::LoggerPtr _logger;
};
typedef IceUtil::Handle<PollingChatSessionI> PollingChatSessionIPtr;

#endif
