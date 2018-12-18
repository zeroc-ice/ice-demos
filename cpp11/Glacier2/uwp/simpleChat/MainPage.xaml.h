// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "MainPage.g.h"
#include "ChatViewPage.xaml.h"
#include "LoginViewPage.xaml.h"

#include <Ice/Ice.h>
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

class Coordinator : public Glacier2::SessionCallback,
                    public Demo::ChatCallback,
                    public std::enable_shared_from_this<Coordinator>
{
public:

    Coordinator();

    void initialize(Windows::UI::Core::CoreDispatcher^);

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
    virtual void message(std::string data, const Ice::Current&);

    //
    // Chat session.
    //
    void say(const std::string&);
    void logout();

    void destroy();

private:

    std::shared_ptr<Demo::ChatSessionPrx> _chat;
    std::shared_ptr<Glacier2::SessionFactoryHelper> _factory;
    std::shared_ptr<Glacier2::SessionHelper> _session;
    LoginData _loginData;
};

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

    std::shared_ptr<Coordinator> coordinator()
    {
        return _coordinator;
    }
    std::shared_ptr<Coordinator> _coordinator;

    static MainPage^ _instance;

    friend ref class LoginViewPage;
    friend ref class ChatViewPage;
    friend class Coordinator;

    LoginViewPage^ _loginView;
    ChatViewPage^ _chatView;
    void signoutClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};

}
