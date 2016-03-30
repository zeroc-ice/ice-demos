// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "MainPage.g.h"
#include "ChatViewPage.xaml.h"
#include "LoginViewPage.xaml.h"

#include <Glacier2/Glacier2.h>
#include <Chat.h>

namespace simpleChat
{
struct LoginData
{
    std::string hostname;
    std::string username;
    std::string password;
};

class Coordinator : public Glacier2::SessionCallback, public Demo::ChatCallback
{
public:

    Coordinator(Windows::UI::Core::CoreDispatcher^);

    void signIn(const LoginData&);
    LoginData loginData();

    //
    //  Session callback
    //
    virtual void createdCommunicator(const Glacier2::SessionHelperPtr&);
    virtual void connected(const Glacier2::SessionHelperPtr&);
    virtual void disconnected(const Glacier2::SessionHelperPtr&);
    virtual void connectFailed(const Glacier2::SessionHelperPtr&, const Ice::Exception&);

    //
    // Chat callback
    //
    virtual void message(const std::string& data, const Ice::Current&);

    //
    // Chat session.
    //
    void say(const std::string&);
    void logout();

    void destroy();

private:

    Demo::ChatSessionPrx _chat;
    Glacier2::SessionFactoryHelperPtr _factory;
    Glacier2::SessionHelperPtr _session;
    LoginData _loginData;
};
typedef IceUtil::Handle<Coordinator> CoordinatorPtr;

public ref class MainPage sealed
{
public:

    MainPage();

    static MainPage^ instance();
    void setConnected(bool c);
    void appendMessage(Platform::String^ msg);
	void suspended();

private:

    virtual void setError(const std::string&);

    CoordinatorPtr coordinator()
    {
        return _coordinator;
    }
    CoordinatorPtr _coordinator;

    static MainPage^ _instance;

    friend ref class LoginViewPage;
	friend ref class ChatViewPage;
    friend class Coordinator;

	LoginViewPage^ _loginView;
	ChatViewPage^ _chatView;
    void signoutClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};

}
