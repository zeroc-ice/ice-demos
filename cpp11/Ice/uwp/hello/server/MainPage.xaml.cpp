// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"
#include <Hello.h>

using namespace std;
using namespace hello;
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

namespace
{

class HelloI : public Hello
{
public:

    HelloI(MainPage^ page) : _page(page)
    {
    }

    virtual void
    sayHello(int delay, const Ice::Current&)
    {
        if(delay > 0)
        {
            this_thread::sleep_for(chrono::milliseconds(delay));
        }

        _page->Dispatcher->RunAsync(
            CoreDispatcherPriority::Normal, ref new DispatchedHandler([=]()
            {
                _page->print(ref new String(L"Hello World!\n"));
            }, CallbackContext::Any));
    }

    void shutdown(const ::Ice::Current& current)
    {
        _page->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,
                                    ref new DispatchedHandler([=]()
                                                              {
                                                                  current.adapter->getCommunicator()->destroy();
                                                                  App::Current->Exit();
                                                              }, CallbackContext::Any));
    }

private:

    MainPage^ _page;
};

}

MainPage::MainPage()
{
    InitializeComponent();
    Ice::registerIceUDP();
}

void
MainPage::resume()
{
    if(!_communicator)
    {
        try
        {

            Ice::InitializationData id;
            id.properties = Ice::createProperties();
            id.properties->setProperty("Hello.Endpoints", "tcp -p 10000:udp -p 10000");
            id.properties->setProperty("Ice.Trace.Network", "2");

            _communicator = Ice::initialize(id);
            auto adapter = _communicator->createObjectAdapter("Hello");
            adapter->add(make_shared<HelloI>(this), Ice::stringToIdentity("hello"));
            adapter->activate();
            print(ref new String(L"Ready to receive requests\n"));
        }
        catch(const std::exception& ex)
        {
            ostringstream os;
            os << "Server initialization failed with exception:\n";
            os << ex.what();
            wstring msg = Ice::stringToWstring(os.str());
            print(ref new String(msg.c_str()));
        }
    }
}

void
MainPage::clear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    output->Text = ref new String(L"");
}

void
MainPage::print(Platform::String^ message)
{
    output->Text = String::Concat(output->Text, message);
}
