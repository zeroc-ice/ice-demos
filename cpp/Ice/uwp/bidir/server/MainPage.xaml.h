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

class CallbackSenderI : public Demo::CallbackSender
{
public:

    CallbackSenderI(MainPage^ page, const std::shared_ptr<Ice::Communicator>& communicator) : _page(page),
		_communicator(communicator)
    {
    }

    virtual void addClient(const Ice::Identity&, const Ice::Current&);
	void start();
	void destroy();
    
private:
        
    MainPage^ _page;
	std::shared_ptr<Ice::Communicator> _communicator;
	bool _destroy;
	std::set<std::shared_ptr<Demo::CallbackReceiverPrx>> _clients;
	std::mutex _mutex;
	std::condition_variable _cv;
	std::thread _senderThread;
};

public ref class MainPage sealed
{
public:
    MainPage();

private:

    friend class CallbackSenderI;

    void print(const std::string&);
	void destroy();

    std::shared_ptr<Ice::Communicator> _communicator;
	std::shared_ptr<Ice::ObjectAdapter> _adapter;
	std::shared_ptr<CallbackSenderI> _sender;
};

}
