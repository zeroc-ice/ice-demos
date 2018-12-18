// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "MainPage.g.h"
#include <Ice/Ice.h>
#include <Callback.h>
#include <future>

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
    callback(int, const Ice::Current&) override;

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
    void dispatch(std::function<void()>);
    Ice::CommunicatorHolder _communicator;
    std::future<void> _start;
};

}
