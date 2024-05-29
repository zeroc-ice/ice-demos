//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "ChatSessionI.h"
#include <Ice/Ice.h>
#include <iostream>
#include <list>

using namespace std;
using namespace Demo;

//
// ChatRoom helper singleton
//

class ChatRoom
{
public:
    static ChatRoom* instance();

    void enter(ChatSessionPrx, ChatCallbackPrx, const Ice::Current&);
    void leave(ChatCallbackPrx, const Ice::Current&);
    void message(const string&) const;
    void deadRouter(const shared_ptr<Ice::Connection>&);
    void destroy();

private:
    mutex _mutex;
    list<ChatCallbackPrx> _callbacks;

    //
    // Map of connection from Glacier2 to proxies of Chat Sessions in this server
    //
    map<shared_ptr<Ice::Connection>, list<ChatSessionPrx>> _connectionMap;
};

ChatRoom*
ChatRoom::instance()
{
    static ChatRoom chatRoomInstance;
    return &chatRoomInstance;
}

void
ChatRoom::enter(ChatSessionPrx session, ChatCallbackPrx callback, const Ice::Current& current)
{
    const lock_guard<mutex> sync(_mutex);
    _callbacks.push_back(callback);

    auto p = _connectionMap.find(current.con);
    if (p == _connectionMap.end())
    {
        cout << "enter: create new entry in connection map" << endl;

        _connectionMap[current.con].push_back(session);

        current.con->setCloseCallback(
            [this](const shared_ptr<Ice::Connection>& con)
            {
                //
                // The Glacier2 router died or otherwise closed its connection
                // to this server, so we cleanup all associated sessions
                //
                deadRouter(con);
            });
    }
    else
    {
        cout << "enter: add session to existing connection map entry" << endl;
        p->second.push_back(session);
    }
}

void
ChatRoom::leave(ChatCallbackPrx callback, const Ice::Current& current)
{
    const lock_guard<mutex> sync(_mutex);

    _callbacks.remove_if([callback = std::move(callback)](const ChatCallbackPrx& cb)
                         { return Ice::proxyIdentityEqual(callback, cb); });
    _connectionMap[current.con].remove_if([&current](const ChatSessionPrx& s)
                                          { return current.id == s->ice_getIdentity(); });
}

void
ChatRoom::message(const string& data) const
{
    const lock_guard<mutex> sync(const_cast<mutex&>(_mutex));
    for (const auto& cb : _callbacks)
    {
        cb->messageAsync(data);
    }
}

void
ChatRoom::deadRouter(const shared_ptr<Ice::Connection>& con)
{
    try
    {
        con->throwException();
    }
    catch (const Ice::ObjectAdapterDeactivatedException&)
    {
        // Ignore server is being shutdown
    }
    catch (...)
    {
        cout << "Detected dead router - destroying all associated sessions " << endl;

        list<ChatSessionPrx> sessions;
        {
            const lock_guard<mutex> sync(_mutex);
            auto p = _connectionMap.find(con);
            if (p != _connectionMap.end())
            {
                sessions.swap(p->second);
                _connectionMap.erase(p);
            }
        }
        for (auto& s : sessions)
        {
            //
            // Collocated calls to the Chat Sessions
            //
            s->destroy();
        }
    }
}

void
ChatRoom::destroy()
{
    //
    // We could also destroy each session first
    //
    const lock_guard<mutex> sync(_mutex);
    _callbacks.clear();
    _connectionMap.clear();
}

//
// ChatSessionI
//

ChatSessionI::ChatSessionI(string userId) : _userId(std::move(userId)) {}

void
ChatSessionI::setCallback(optional<ChatCallbackPrx> callback, const Ice::Current& current)
{
    const lock_guard<mutex> sync(_mutex);
    if (!_callback && callback)
    {
        _callback = callback;
        auto chatRoom = ChatRoom::instance();
        chatRoom->message(_userId + " has entered the chat room.");
        chatRoom->enter(ChatSessionPrx(current.adapter->createProxy(current.id)), std::move(*callback), current);
    }
}

void
ChatSessionI::say(string data, const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);
    ChatRoom::instance()->message(_userId + " says: " + data);
}

void
ChatSessionI::destroy(const Ice::Current& current)
{
    cout << "Destroying session for " << _userId << endl;
    const lock_guard<mutex> sync(_mutex);
    if (_callback)
    {
        auto chatRoom = ChatRoom::instance();
        chatRoom->leave(*_callback, current);
        _callback = nullopt;
        chatRoom->message(_userId + " has left the chat room.");
    }
    current.adapter->remove(current.id);
}

//
// ChatSessionManagerI
//

optional<Glacier2::SessionPrx>
ChatSessionManagerI::create(string userId, optional<Glacier2::SessionControlPrx>, const Ice::Current& current)
{
    const Ice::Identity ident = {Ice::generateUUID(), "session"};
    return Ice::uncheckedCast<Glacier2::SessionPrx>(current.adapter->add(make_shared<ChatSessionI>(userId), ident));
}

void
ChatSessionManagerI::destroy()
{
    ChatRoom::instance()->destroy();
}
