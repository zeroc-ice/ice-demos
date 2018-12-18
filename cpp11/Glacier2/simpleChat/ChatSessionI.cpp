// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ChatSessionI.h>
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

    void enter(const shared_ptr<ChatSessionPrx>&, const shared_ptr<ChatCallbackPrx>&, const Ice::Current&);
    void leave(const shared_ptr<ChatCallbackPrx>&, const Ice::Current&);
    void message(const string&) const;
    void deadRouter(const shared_ptr<Ice::Connection>&);
    void destroy();

private:

    mutex _mutex;
    list<shared_ptr<ChatCallbackPrx>> _callbacks;

    //
    // Map of connection from Glacier2 to proxies of Chat Sessions in this server
    //
    map<shared_ptr<Ice::Connection>, list<shared_ptr<ChatSessionPrx>>> _connectionMap;
};

ChatRoom*
ChatRoom::instance()
{
    static ChatRoom chatRoomInstance;
    return &chatRoomInstance;
}

void
ChatRoom::enter(const shared_ptr<ChatSessionPrx>& session, const shared_ptr<ChatCallbackPrx>& callback,
                const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);
    _callbacks.push_back(callback);

    auto p = _connectionMap.find(current.con);
    if(p == _connectionMap.end())
    {
        cout << "enter: create new entry in connection map" << endl;

        _connectionMap[current.con].push_back(session);

        //
        // Never close this connection from Glacier2 and turn on heartbeats with a timeout of 30s
        //
        current.con->setACM(30, Ice::ACMClose::CloseOff, Ice::ACMHeartbeat::HeartbeatAlways);

        current.con->setCloseCallback([this](const shared_ptr<Ice::Connection>& con)
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
ChatRoom::leave(const shared_ptr<ChatCallbackPrx>& callback, const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);

    _callbacks.remove_if([&callback](const shared_ptr<ChatCallbackPrx>& cb) { return Ice::proxyIdentityEqual(callback, cb); });
    _connectionMap[current.con].remove_if([&current](const shared_ptr<ChatSessionPrx>& s) { return current.id == s->ice_getIdentity(); });
}

void
ChatRoom::message(const string& data) const
{
    lock_guard<mutex> sync(const_cast<mutex&>(_mutex));
    for(const auto& cb : _callbacks)
    {
        cb->messageAsync(data);
    }
}

void
ChatRoom::deadRouter(const shared_ptr<Ice::Connection>& con)
{
    cout << "Detected dead router - destroying all associated sessions " << endl;

    list<shared_ptr<ChatSessionPrx>> sessions;
    {
        lock_guard<mutex> sync(_mutex);
        auto p = _connectionMap.find(con);
        if(p != _connectionMap.end())
        {
            sessions.swap(p->second);
            _connectionMap.erase(p);
        }
    }
    for(auto& s : sessions)
    {
        //
        // Collocated calls to the Chat Sessions
        //
        s->destroy();
    }
}

void
ChatRoom::destroy()
{
    //
    // We could also destroy each session first
    //
    lock_guard<mutex> sync(_mutex);
    _callbacks.clear();
    _connectionMap.clear();
}

//
// ChatSessionI
//

ChatSessionI::ChatSessionI(const string& userId) :
    _userId(userId)
{
}

void
ChatSessionI::setCallback(shared_ptr<ChatCallbackPrx> callback, const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);
    if(!_callback)
    {
        _callback = callback;
        auto chatRoom = ChatRoom::instance();
        chatRoom->message(_userId + " has entered the chat room.");
        chatRoom->enter(Ice::uncheckedCast<ChatSessionPrx>(current.adapter->createProxy(current.id)), callback, current);
    }
}

void
ChatSessionI::say(string data, const Ice::Current&)
{
    lock_guard<mutex> sync(_mutex);
    ChatRoom::instance()->message(_userId + " says: " + data);
}

void
ChatSessionI::destroy(const Ice::Current& current)
{
    cout << "Destroying session for " << _userId << endl;
    lock_guard<mutex> sync(_mutex);
    if(_callback)
    {
        auto chatRoom = ChatRoom::instance();
        chatRoom->leave(_callback, current);
        _callback = nullptr;
        chatRoom->message(_userId + " has left the chat room.");
    }
    current.adapter->remove(current.id);
}

//
// ChatSessionManagerI
//

shared_ptr<Glacier2::SessionPrx>
ChatSessionManagerI::create(string userId, shared_ptr<Glacier2::SessionControlPrx>, const Ice::Current& current)
{
    Ice::Identity ident = { Ice::generateUUID(), "session" };
    return Ice::uncheckedCast<Glacier2::SessionPrx>(current.adapter->add(make_shared<ChatSessionI>(userId), ident));
}

void
ChatSessionManagerI::destroy()
{
    ChatRoom::instance()->destroy();
}
