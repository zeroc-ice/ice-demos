// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatSessionManagerI.h>
#include <ChatSessionI.h>
#include <ChatUtils.h>

using namespace std;

ChatSessionManagerI::ChatSessionManagerI(const shared_ptr<ChatRoom>& chatRoom, bool trace,
                                         const shared_ptr<Ice::Logger>& logger) :
    _chatRoom(chatRoom),
    _trace(trace),
    _logger(logger)
{
}

shared_ptr<Glacier2::SessionPrx>
ChatSessionManagerI::create(string name,
                            shared_ptr<Glacier2::SessionControlPrx> sessionControl,
                            const Ice::Current& current)
{
    string vname;
    try
    {
        vname = validateName(name);
        _chatRoom->reserve(vname);
    }
    catch(const exception& ex)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "Cannot create push session:\n" << ex;
        }
        throw Glacier2::CannotCreateSessionException(ex.what());
    }

    shared_ptr<Glacier2::SessionPrx> proxy;
    try
    {
        auto session = make_shared<ChatSessionI>(_chatRoom, vname, _trace, _logger);
        proxy = Ice::uncheckedCast<Glacier2::SessionPrx>(current.adapter->addWithUUID(session));

        Ice::IdentitySeq ids;
        ids.push_back(proxy->ice_getIdentity());
        sessionControl->identities()->add(ids);
    }
    catch(const Ice::LocalException& ex)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "Cannot create push session for user '" << vname << "':\n" << ex;
        }
        if(proxy)
        {
            proxy->destroy();
        }
        throw Glacier2::CannotCreateSessionException("internal server error");
    }

    if(_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "Push session created for user '" << vname << "'.";
    }
    return proxy;
}
