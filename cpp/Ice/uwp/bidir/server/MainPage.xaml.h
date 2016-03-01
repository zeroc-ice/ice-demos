// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "MainPage.g.h"
#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <Callback.h>

namespace bidir
{
ref class MainPage;

class CallbackSenderI : public Demo::CallbackSender, public IceUtil::Thread, public IceUtil::Monitor<IceUtil::Mutex>
{
public:

    CallbackSenderI(MainPage^ page, const Ice::CommunicatorPtr& communicator) : _page(page),
		_communicator(communicator)
    {
    }

    virtual void addClient(const Ice::Identity&, const Ice::Current&);
	virtual void run();
    
private:
        
    MainPage^ _page;
	Ice::CommunicatorPtr _communicator;
	std::set<Demo::CallbackReceiverPrx> _clients;
};
typedef IceUtil::Handle<CallbackSenderI> CallbackSenderIPtr;

public ref class MainPage sealed
{
public:
    MainPage();

private:

    friend class CallbackSenderI;

    void print(const std::string&);

    Ice::CommunicatorPtr _communicator;
	Ice::ObjectAdapterPtr _adapter;
};

}
