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

class CallbackReceiverI : public Demo::CallbackReceiver
{
public:

    CallbackReceiverI(MainPage^ page) : _page(page)
    {
    }

    virtual void
    callback(Ice::Int, const Ice::Current&);
    
private:
        
    MainPage^ _page;
};

public ref class MainPage sealed
{
public:
    MainPage();

private:

    friend class CallbackReceiverI;

    void callback(Ice::Int, const Ice::Current&);
    void print(const std::string&);
    void startClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void stopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    Ice::CommunicatorPtr _communicator;
};

}
