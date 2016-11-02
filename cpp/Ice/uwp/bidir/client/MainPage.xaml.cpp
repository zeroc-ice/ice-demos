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
CallbackReceiverI::callback(Ice::Int num, const Ice::Current& current)
{
    _page->callback(num, current);
}

MainPage::MainPage()
{
    InitializeComponent();
}

void bidir::MainPage::startClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    try
    {
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->setProperty("Ice.Warn.Connections", "1");

        _communicator = Ice::initialize(initData);

        auto server = Ice::checkedCast<CallbackSenderPrx>(
            _communicator->stringToProxy("sender:tcp -h " + Ice::wstringToString(hostname->Text->Data()) + 
                                         " -p 10000"));

        if(!server)
        {
            print("invalid proxy\n");
            return;
        }
        startClient->IsEnabled = false;
        stopClient->IsEnabled = true;
        Ice::ObjectAdapterPtr adapter = _communicator->createObjectAdapter("");
        Ice::Identity ident;
        ident.name = Ice::generateUUID();
        ident.category = "";
        auto cr = make_shared<CallbackReceiverI>(this);
        adapter->add(cr, ident);
        adapter->activate();
        server->ice_getConnection()->setAdapter(adapter);
        server->addClientAsync(ident, nullptr, 
            [=](const exception_ptr ex)
            {
                try
                {
                    rethrow_exception(ex);
                }
                catch (const exception& err)
                {
                    ostringstream os;
                    os << err.what() << endl;
                    print(os.str());
                    startClient->IsEnabled = true;
                    stopClient->IsEnabled = false;
                }
            });
    }
    catch(const Ice::Exception& ex)
    {
        ostringstream os;
        os << ex << endl;
        print(os.str());
        startClient->IsEnabled = true;
        stopClient->IsEnabled = false;
    }
}


void bidir::MainPage::stopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    try
    {
        if(_communicator)
        {
            _communicator->destroy();
            _communicator = 0;
        }
        startClient->IsEnabled = true;
        stopClient->IsEnabled = false;
    }
    catch(const Ice::Exception& ex)
    {
        ostringstream os;
        os << ex << endl;
        print(os.str());
        startClient->IsEnabled = true;
        stopClient->IsEnabled = false;
    }
}

void
bidir::MainPage::callback(Ice::Int num, const Ice::Current&)
{
    ostringstream os;
    os << "received callback #" << num << endl;
    print(os.str());
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
