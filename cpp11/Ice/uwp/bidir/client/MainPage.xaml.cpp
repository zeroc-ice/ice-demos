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
CallbackReceiverI::callback(Ice::Int num, const Ice::Current&)
{
    ostringstream os;
    os << "received callback #" << num << endl;
    _page->print(os.str());
}

MainPage::MainPage()
{
    InitializeComponent();
}

void
bidir::MainPage::dispatch(function<void()> call)
{
    this->Dispatcher->RunAsync(
                            CoreDispatcherPriority::Normal,
                            ref new DispatchedHandler([=]()
                                {
                                    call();
                                },
                            CallbackContext::Any));
}

void bidir::MainPage::startClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties();
    initData.properties->setProperty("Ice.Warn.Connections", "1");
    initData.dispatcher = [=](function<void()> call, const shared_ptr<Ice::Connection>&)
        {
            dispatch(call);
        };

    startClient->IsEnabled = false;
    stopClient->IsEnabled = true;

    string endpoint = "sender:tcp -h " + Ice::wstringToString(hostname->Text->Data()) + " -p 10000";

    _start = async(launch::async, [this, initData, endpoint]
        {
            try
            {
                _communicator = Ice::initialize(initData);
                auto server = Ice::checkedCast<CallbackSenderPrx>(_communicator->stringToProxy(endpoint));

                if(!server)
                {
                    dispatch([this]() { print("invalid proxy\n"); });
                    return;
                }

                //
                // Create an object adapter with no name and no endpoints for receiving callbacks
                // over bidirectional connections.
                //
                auto adapter = _communicator->createObjectAdapter("");

                //
                // Register the callback receiver servant with the object adapter and activate
                // the adapter.
                //
                auto proxy = Ice::uncheckedCast<CallbackReceiverPrx>(
                                        adapter->addWithUUID(make_shared<CallbackReceiverI>(this)));
                adapter->activate();

                //
                // Associate the object adapter with the bidirectional connection.
                //
                server->ice_getConnection()->setAdapter(adapter);

                //
                // Provide the proxy of the callback receiver object to the server and wait for
                // shutdown.
                //
                server->addClientAsync(proxy, nullptr,
                    [=](const exception_ptr ex)
                    {
                        try
                        {
                            rethrow_exception(ex);
                        }
                        catch(const exception& err)
                        {
                            print(err.what());
                            startClient->IsEnabled = true;
                            stopClient->IsEnabled = false;
                        }
                    });
            }
            catch(Platform::Exception^ ex)
            {
                dispatch([this, ex]()
                    {
                        print(wstringToString(ex->Message->Data()));
                        startClient->IsEnabled = true;
                        stopClient->IsEnabled = false;
                    });
            }
            catch(const Ice::Exception& ex)
            {
                string msg(ex.what()); // Copy the message, dispatch is asynchronous
                dispatch([this, msg]()
                    {
                        print(msg);
                        startClient->IsEnabled = true;
                        stopClient->IsEnabled = false;
                    });
            }
        });
}

void bidir::MainPage::stopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    try
    {
        _start.get();
        if(_communicator)
        {
            _communicator->destroy();
            _communicator = nullptr;
        }
    }
    catch(const Ice::Exception& ex)
    {
        print(ex.what());
    }
    startClient->IsEnabled = true;
    stopClient->IsEnabled = false;
}

void
bidir::MainPage::print(const std::string& message)
{
    output->Text += ref new String(Ice::stringToWstring(message).c_str());
    output->UpdateLayout();
    scroller->ChangeView(nullptr, scroller->ScrollableHeight, nullptr);
}
