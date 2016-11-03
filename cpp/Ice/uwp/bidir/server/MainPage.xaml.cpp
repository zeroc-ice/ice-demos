// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"

using namespace bidir;

using namespace std;
using namespace Demo;
using namespace Ice;
using namespace Platform;
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

void
CallbackSenderI::addClient(Ice::Identity ident, const Ice::Current& current)
{
    unique_lock<mutex> lock(_mutex);

    ostringstream os;
    os << "adding client `";
    os << Ice::identityToString(ident);
    os << "'\n";
    _page->print(os.str());

    auto client = Ice::uncheckedCast<CallbackReceiverPrx>(current.con->createProxy(ident));
    _clients.insert(client);
}

void
CallbackSenderI::start()
{
    thread t([this]()
        {
            int num = 0;
            while(true)
            {
                set<shared_ptr<Demo::CallbackReceiverPrx>> clients;
                {
                    unique_lock<mutex> lock(this->_mutex);
                    this->_cv.wait_for(lock, chrono::seconds(2));
                    clients = this->_clients;
                }

                if(!clients.empty())
                {
                    ++num;
                    for(auto p : clients)
                    {
                        try
                        {
                            p->callback(num);
                        }
                        catch(const Ice::Exception& ex)
                        {
                            ostringstream os;
                            os << "removing client `";
                            os << Ice::identityToString(p->ice_getIdentity());
                            os << "':\n";
                            os << ex;
                            os << "\n";
                            _page->print(os.str());

                            unique_lock<mutex> lock(_mutex);
                            this->_clients.erase(p);
                        }
                    }
                }
            }
        });
    _senderThread = move(t);
}

MainPage::MainPage()
{
    InitializeComponent();
    try
    {
        Ice::InitializationData id;
        id.properties = Ice::createProperties();
        id.properties->setProperty("Callback.Server.Endpoints", "tcp -p 10000:ws -p 10002");
        id.properties->setProperty("Ice.Trace.Network", "2");

        _communicator = Ice::initialize(id);
        _adapter = _communicator->createObjectAdapter("Callback.Server");
        _sender = make_shared<CallbackSenderI>(this, _communicator);
        _adapter->add(_sender, Ice::stringToIdentity("sender"));
        _adapter->activate();

        _sender->start();
    }
    catch(const std::exception& ex)
    {
        ostringstream os;
        os << "Server initialization failed with exception:\n";
        os << ex.what();
        print(os.str());
    }
}

void 
bidir::MainPage::print(const std::string& message)
{
    this->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, 
                    ref new DispatchedHandler(
                            [=] ()
                                {
                                    output->Text += ref new String(Ice::stringToWstring(message).c_str());
                                    output->UpdateLayout();
                                    scroller->ChangeView(nullptr, scroller->ScrollableHeight, nullptr);
                                }, 
                            CallbackContext::Any));
}
