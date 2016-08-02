// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"

using namespace simpleChat;

using namespace Platform;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;

using namespace std;

MainPage^ MainPage::_instance = nullptr;

Coordinator::Coordinator(CoreDispatcher^ dispatcher)
{
    Ice::InitializationData id;
    id.properties = Ice::createProperties();
    id.dispatcher = Ice::newDispatcher(
        [=](const Ice::DispatcherCallPtr& call, const Ice::ConnectionPtr&)
            {
                dispatcher->RunAsync(
                    CoreDispatcherPriority::Normal, ref new DispatchedHandler([=]()
                        {
                            call->run();
                        }, CallbackContext::Any));
            });
    _factory = new Glacier2::SessionFactoryHelper(id, this);

    Ice::Identity identity;
    identity.name = "router";
    identity.category = "DemoGlacier2";
    _factory->setRouterIdentity(identity);
}

void
Coordinator::signIn(const LoginData& loginData)
{
    _loginData = loginData;
    _factory->setRouterHost(_loginData.hostname);
    _session = _factory->connect(_loginData.username, _loginData.password);
}

LoginData
Coordinator::loginData()
{
    return _loginData;
}

void
Coordinator::say(const std::string& msg)
{
    try
    {
        _chat->begin_say(msg, nullptr, [](const Ice::Exception& ex)
                                            {
                                                ostringstream os;
                                                os << "Connect failed:\n" << ex << endl;
                                                MainPage::instance()->setError(os.str());
                                            });
    }
    catch(const Ice::CommunicatorDestroyedException& ex)
    {
        ostringstream os;
        os << "Connect failed:\n" << ex << endl;
        MainPage::instance()->setError(os.str());
    }
}

void
Coordinator::createdCommunicator(const Glacier2::SessionHelperPtr&)
{
}

void
Coordinator::connected(const Glacier2::SessionHelperPtr& session)
{
    if(_session != session)
    {
        return;
    }
    try
    {
        _chat = Demo::ChatSessionPrx::uncheckedCast(session->session());
        _chat->begin_setCallback(Demo::ChatCallbackPrx::uncheckedCast(_session->addWithUUID(this)),
                                 []()
                                    {
                                         MainPage::instance()->setConnected(true);
                                    },
                                 [](const Ice::Exception& ex)
                                    {
                                        ostringstream os;
                                        os << "Connect failed:\n" << ex << endl;
                                        MainPage::instance()->setError(os.str());
                                    });
    }
    catch(const Ice::CommunicatorDestroyedException& ex)
    {
        ostringstream os;
        os << "Connect failed:\n" << ex << endl;
        MainPage::instance()->setError(os.str());
    }
}

void
Coordinator::disconnected(const Glacier2::SessionHelperPtr&)
{
    MainPage::instance()->setConnected(false);
}

void
Coordinator::connectFailed(const Glacier2::SessionHelperPtr&, const Ice::Exception& ex)
{
    ostringstream os;
    os << "Connect failed:\n" << ex << endl;
    MainPage::instance()->setError(os.str());
}

void
Coordinator::message(const string& msg, const Ice::Current&)
{
    try
    {
        MainPage::instance()->appendMessage(ref new String(Ice::stringToWstring(msg).c_str()));
    }
    catch(const Ice::CommunicatorDestroyedException& ex)
    {
        ostringstream os;
        os << ex << endl;
        MainPage::instance()->setError(os.str());
    }
}

void
Coordinator::logout()
{
    if(_session)
    {
        _session->destroy();
        _session = 0;
    }
}

void
Coordinator::destroy()
{
    _factory->destroy();
}

MainPage::MainPage()
{
    InitializeComponent();
    _instance = this;
    _coordinator = new Coordinator(this->Dispatcher);
    setConnected(false);
}

void
MainPage::appendMessage(String^ message)
{
    _chatView->appendMessage(message);
}

MainPage^
MainPage::instance()
{
    return _instance;
}

void
MainPage::setConnected(bool connected)
{
    String^ pageName = nullptr;
    if(connected)
    {
        pageName = "simpleChat.ChatViewPage";
        signout->Visibility =  Windows::UI::Xaml::Visibility::Visible;
    }
    else
    {
        pageName = "simpleChat.LoginViewPage";
        signout->Visibility =  Windows::UI::Xaml::Visibility::Collapsed;
    }
    TypeName page = {pageName, TypeKind::Custom};
    main->Navigate(page, this);
    _loginView->signinCompleted();
}

void
MainPage::setError(const std::string& err)
{
    setConnected(false);
    _loginView->setError(ref new String(Ice::stringToWstring(err).c_str()));
}

void
MainPage::signoutClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    _coordinator->logout();
}

void
MainPage::suspended()
{
    _coordinator->destroy();
}
