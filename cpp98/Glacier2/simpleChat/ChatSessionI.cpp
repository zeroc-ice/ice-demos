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

    void enter(const Demo::ChatSessionPrx&, const Demo::ChatCallbackPrx&, const Ice::Current&);
    void leave(const Demo::ChatCallbackPrx&, const Ice::Current&);
    void message(const string&) const;
    void deadRouter(const Ice::ConnectionPtr&);
    void destroy();

private:

    IceUtil::Mutex _mutex;
    list<ChatCallbackPrx> _callbacks;
    map<Ice::ConnectionPtr, list<ChatSessionPrx> > _connectionMap;
};

namespace
{

class ClosedCallbackI : public Ice::CloseCallback
{
public:

    //
    // The Glacier2 router died or otherwise closed its connection
    // to this server, so we cleanup all associated sessions
    //
    virtual void closed(const Ice::ConnectionPtr& con)
    {
        ChatRoom::instance()->deadRouter(con);
    }
};

struct IdentityMatch
{
    IdentityMatch(const Ice::Identity& identity) :
        _identity(identity)
    {
    }

    bool operator()(const Ice::ObjectPrx& prx) const
    {
        return prx->ice_getIdentity() == _identity;
    }

    Ice::Identity _identity;
};

}

ChatRoom*
ChatRoom::instance()
{
    //
    // Assume a C++ compiler that implements thread-safe or "magic" local statics.
    // If we use a C++ compiler without this guarantee, we need a different implementation.
    //
    static ChatRoom chatRoomInstance;
    return &chatRoomInstance;
}

void
ChatRoom::enter(const Demo::ChatSessionPrx& session, const ChatCallbackPrx& callback, const Ice::Current& current)
{
    IceUtil::Mutex::Lock sync(_mutex);
    _callbacks.push_back(callback);

    map<Ice::ConnectionPtr, list<ChatSessionPrx> >::iterator p = _connectionMap.find(current.con);
    if(p == _connectionMap.end())
    {
        cout << "enter: create new entry in connection map" << endl;

        _connectionMap[current.con].push_back(session);

        //
        // Never close this connection from Glacier2 and turn on heartbeats with a timeout of 30s
        //
        current.con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);

        current.con->setCloseCallback(new ClosedCallbackI);
    }
    else
    {
        cout << "enter: add session to existing connection map entry" << endl;
        p->second.push_back(session);
    }
}

void
ChatRoom::leave(const ChatCallbackPrx& callback, const Ice::Current& current)
{
    IceUtil::Mutex::Lock sync(_mutex);

    _callbacks.remove_if(IdentityMatch(callback->ice_getIdentity()));
    _connectionMap[current.con].remove_if(IdentityMatch(current.id));
}

void
ChatRoom::message(const string& data) const
{
    IceUtil::Mutex::Lock sync(_mutex);
    for(list<ChatCallbackPrx>::const_iterator p = _callbacks.begin(); p != _callbacks.end(); ++p)
    {
        (*p)->begin_message(data);
    }
}

void
ChatRoom::deadRouter(const Ice::ConnectionPtr& con)
{
    cout << "Detected dead router - destroying all associated sessions " << endl;

    list<ChatSessionPrx> sessions;
    {
        IceUtil::Mutex::Lock sync(_mutex);
        map<Ice::ConnectionPtr, list<ChatSessionPrx> >::iterator p = _connectionMap.find(con);
        if(p != _connectionMap.end())
        {
            sessions.swap(p->second);
            _connectionMap.erase(p);
        }
    }
    for(list<ChatSessionPrx>::const_iterator s = sessions.begin(); s != sessions.end(); ++s)
    {
        //
        // Collocated calls to the Chat Sessions
        //
        (*s)->destroy();
    }
}

void
ChatRoom::destroy()
{
    //
    // We could also destroy each session first
    //
    IceUtil::Mutex::Lock sync(_mutex);
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
ChatSessionI::setCallback(const ChatCallbackPrx& callback, const Ice::Current& current)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(!_callback)
    {
        _callback = callback;
        ChatRoom* chatRoom = ChatRoom::instance();
        chatRoom->message(_userId + " has entered the chat room.");
        chatRoom->enter(ChatSessionPrx::uncheckedCast(current.adapter->createProxy(current.id)), callback, current);
    }
}

void
ChatSessionI::say(const string& data, const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    ChatRoom::instance()->message(_userId + " says: " + data);
}

void
ChatSessionI::destroy(const Ice::Current& current)
{
    cout << "Destroying session for " << _userId << endl;
    IceUtil::Mutex::Lock sync(_mutex);
    if(_callback)
    {
        ChatRoom* chatRoom = ChatRoom::instance();
        chatRoom->leave(_callback, current);
        _callback = 0;
        chatRoom->message(_userId + " has left the chat room.");
    }
    current.adapter->remove(current.id);
}

//
// ChatSessionManagerI
//

Glacier2::SessionPrx
ChatSessionManagerI::create(const string& userId, const Glacier2::SessionControlPrx&, const Ice::Current& current)
{
    Ice::Identity ident = { Ice::generateUUID(), "session" };
    return Glacier2::SessionPrx::uncheckedCast(current.adapter->add(new ChatSessionI(userId), ident));
}

void
ChatSessionManagerI::destroy()
{
    ChatRoom::instance()->destroy();
}
