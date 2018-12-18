// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatRoom.h>

using namespace std;

ChatRoom::ChatRoom(bool trace, const shared_ptr<Ice::Logger>& logger) :
    _trace(trace),
    _logger(logger)
{
}

void
ChatRoom::reserve(const string& name)
{
    lock_guard<mutex> sync(_mutex);
    if(_reserved.find(name) != _reserved.end() || _members.find(name) != _members.end())
    {
        throw runtime_error("The name " + name + " is already in use.");
    }
    _reserved.insert(name);
}

void
ChatRoom::unreserve(const string& name)
{
    lock_guard<mutex> sync(_mutex);
    _reserved.erase(name);
}

void
ChatRoom::join(const string& name, const shared_ptr<ChatRoomCallbackAdapter>& callback)
{
    lock_guard<mutex> sync(_mutex);
    long long timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    _reserved.erase(name);

    Ice::StringSeq names;
    for(const auto& q : _members)
    {
        names.push_back(q.first);
    }

    callback->init(move(names));

    _members[name] = callback;

    auto e = make_shared<PollingChat::UserJoinedEvent>(timestamp, name);
    for(const auto& q: _members)
    {
        q.second->join(e);
    }

    if(_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "User '" << name << "' joined the chat room.";
    }
}

void
ChatRoom::leave(const string& name)
{
    lock_guard<mutex> sync(_mutex);
    long long timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    _members.erase(name);

    auto e = make_shared<PollingChat::UserLeftEvent>(timestamp, name);
    for(const auto& q: _members)
    {
        q.second->leave(e);
    }

    if(_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "User '" << name << "' left the chat room.";
    }
}

Ice::Long
ChatRoom::send(const string& name, string message)
{
    lock_guard<mutex> sync(_mutex);
    long long timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    auto e = make_shared<PollingChat::MessageEvent>(timestamp, name, message);
    for(const auto& q: _members)
    {
        q.second->send(e);
    }
    if(_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "User '" << name << "' sent a message to the chat room.";
    }
    return timestamp;
}
