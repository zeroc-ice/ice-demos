// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatSessionManagerI.h>
#include <ChatSessionI.h>
#include <ChatUtils.h>

using namespace std;

ChatSessionManagerI::ChatSessionManagerI(const ChatRoomPtr& chatRoom, bool trace, const Ice::LoggerPtr& logger) :
    _chatRoom(chatRoom),
    _trace(trace),
    _logger(logger)
{
}

Glacier2::SessionPrx
ChatSessionManagerI::create(const string& name,
                            const Glacier2::SessionControlPrx& sessionControl,
                            const Ice::Current& c)
{
    string vname;
    try
    {
        vname = validateName(name);
        _chatRoom->reserve(vname);
    }
    catch(const string& reason)
    {
        if(_trace)
        {
            ostringstream os;
            os << "Cannot create push session:\n" << reason;
            _logger->trace("info", os.str());
        }
        throw Glacier2::CannotCreateSessionException(reason);
    }

    Glacier2::SessionPrx proxy;
    try
    {
        ChatSessionIPtr session = new ChatSessionI(_chatRoom, vname, _trace, _logger);
        proxy = Glacier2::SessionPrx::uncheckedCast(c.adapter->addWithUUID(session));

        Ice::IdentitySeq ids;
        ids.push_back(proxy->ice_getIdentity());
        sessionControl->identities()->add(ids);
    }
    catch(const Ice::LocalException& ex)
    {
        if(_trace)
        {
            ostringstream os;
            os << "Cannot create push session for user '" << vname << "':\n" << ex;
            _logger->trace("info", os.str());
        }
        if(proxy)
        {
            proxy->destroy();
        }
        throw Glacier2::CannotCreateSessionException("internal server error");
    }

    if(_trace)
    {
        ostringstream os;
        os << "Push session created for user '" << vname << "'.";
        _logger->trace("info", os.str());
    }
    return proxy;
}
