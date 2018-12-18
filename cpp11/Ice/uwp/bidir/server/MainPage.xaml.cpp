// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"

using namespace bidir;

using namespace std;
using namespace Demo;
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

CallbackSenderI::CallbackSenderI(MainPage^ page) :
    _page(page),
    _destroy(false)
{
}

void
CallbackSenderI::addClient(std::shared_ptr<CallbackReceiverPrx> client, const Ice::Current& current)
{
    lock_guard<mutex> lock(_mutex);

    ostringstream os;
    os << "adding client `" << Ice::identityToString(client->ice_getIdentity()) << "'\n";
    _page->print(os.str());

    _clients.push_back(client->ice_fixed(current.con));
}

void
CallbackSenderI::start()
{
    unique_lock<mutex> lock(_mutex);
    _destroy = false;
    _result = async(launch::async,
                    [this]
                    {
                        invokeCallback();
                    });
}

void
CallbackSenderI::stop()
{
    {
        unique_lock<mutex> lock(_mutex);
        if(_destroy)
        {
            return;
        }
        _destroy = true;
    }
    _result.wait();
}

void
CallbackSenderI::invokeCallback()
{
    int num = 0;
    unique_lock<mutex> lock(_mutex);
    while(!_destroy)
    {
        _cv.wait_for(lock, chrono::seconds(2));

        if(!_destroy && !_clients.empty())
        {
            ++num;

            //
            // Invoke callback on all clients; it's safe to do it with _mutex locked
            // because Ice guarantees these async invocations never block the calling thread
            //
            // The exception callback, if called, is called by a thread from the Ice client
            // thread pool, and never the calling thread
            for(const auto& p: _clients)
            {
                auto self = shared_from_this();
                p->callbackAsync(num, nullptr,
                    [self, p](exception_ptr eptr) { self->removeClient(p, eptr); });
            }
        }
    }
}

void
CallbackSenderI::removeClient(const shared_ptr<CallbackReceiverPrx>& client, exception_ptr eptr)
{
    try
    {
        rethrow_exception(eptr);
    }
    catch(const Ice::Exception& ex)
    {
        cerr << "removing client `" << Ice::identityToString(client->ice_getIdentity()) << "':\n"
             << ex << endl;
    }

    lock_guard<mutex> lock(_mutex);
    auto p = find(_clients.begin(), _clients.end(), client);
    assert(p != _clients.end());
    _clients.erase(p);
}

MainPage::MainPage()
{
    InitializeComponent();
    Ice::registerIceWS();
}

void
MainPage::resume()
{
    try
    {
        if(!_communicator)
        {
            Ice::InitializationData id;
            id.properties = Ice::createProperties();
            id.properties->setProperty("Callback.Server.Endpoints", "tcp -p 10000:ws -p 10002");
            id.properties->setProperty("Ice.Trace.Network", "2");

            _communicator = move(Ice::initialize(id));
            auto adapter = _communicator->createObjectAdapter("Callback.Server");
            _sender = make_shared<CallbackSenderI>(this);
            adapter->add(_sender, Ice::stringToIdentity("sender"));
            adapter->activate();
        }
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
MainPage::suspend()
{
    _sender->stop();
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
