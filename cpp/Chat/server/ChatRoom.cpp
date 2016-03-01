// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatRoom.h>

using namespace std;

ChatRoom::ChatRoom(bool trace, const Ice::LoggerPtr& logger) :
    _trace(trace),
    _logger(logger)
{
}

void
ChatRoom::reserve(const string& name)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_reserved.find(name) != _reserved.end() || _members.find(name) != _members.end())
    {
        throw string("The name " + name + " is already in use.");
    }
    _reserved.insert(name);
}

void
ChatRoom::unreserve(const string& name)
{
    IceUtil::Mutex::Lock sync(_mutex);
    _reserved.erase(name);
}

void
ChatRoom::join(const string& name, const ChatRoomCallbackAdapterPtr& callback)
{
    IceUtil::Mutex::Lock sync(_mutex);
    IceUtil::Int64 timestamp = IceUtil::Time::now().toMilliSeconds();

    _reserved.erase(name);

    Ice::StringSeq names;
    ChatRoomCallbackMap::const_iterator q;
    for(q = _members.begin(); q != _members.end(); ++q)
    {
        names.push_back(q->first);
    }

    callback->init(names);

    _members[name] = callback;

    PollingChat::UserJoinedEventPtr e = new PollingChat::UserJoinedEvent(timestamp, name);
    for(q = _members.begin(); q != _members.end(); ++q)
    {
        q->second->join(e);
    }

    if(_trace)
    {
        ostringstream os;
        os << "User '" << name << "' joined the chat room.";
        _logger->trace("info", os.str());
    }
}

void
ChatRoom::leave(const string& name)
{
    IceUtil::Mutex::Lock sync(_mutex);
    IceUtil::Int64 timestamp = IceUtil::Time::now().toMilliSeconds();

    _members.erase(name);

    PollingChat::UserLeftEventPtr e = new PollingChat::UserLeftEvent(timestamp, name);
    for(ChatRoomCallbackMap::const_iterator q = _members.begin(); q != _members.end(); ++q)
    {
        q->second->leave(e);
    }

    if(_trace)
    {
        ostringstream os;
        os << "User '" << name << "' left the chat room.";
        _logger->trace("info", os.str());
    }
}

Ice::Long
ChatRoom::send(const string& name, const string& message)
{
    IceUtil::Mutex::Lock sync(_mutex);
    IceUtil::Int64 timestamp = IceUtil::Time::now().toMilliSeconds();

    PollingChat::MessageEventPtr e = new PollingChat::MessageEvent(timestamp, name, message);
    for(ChatRoomCallbackMap::iterator q = _members.begin(); q != _members.end(); ++q)
    {
        q->second->send(e);
    }
    if(_trace)
    {
        ostringstream os;
        os << "User '" << name << "' sent a message to the chat room.";
        _logger->trace("info", os.str());
    }
    return timestamp;
}
