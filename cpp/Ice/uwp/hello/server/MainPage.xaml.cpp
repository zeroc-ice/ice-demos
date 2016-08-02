// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"
#include "Hello.h"

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
			IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
		}

		_page->Dispatcher->RunAsync(
			CoreDispatcherPriority::Normal, ref new DispatchedHandler([=]()
			{
				_page->print(ref new String(L"Hello World!\n"));
			}, CallbackContext::Any));
	}

	virtual void
	shutdown(const Ice::Current&)
	{
		Application::Current->Exit();
	}

private:

	MainPage^ _page;
};
}

MainPage::MainPage()
{
    InitializeComponent();

    try
    {
        Ice::InitializationData id;
        id.properties = Ice::createProperties();
        id.properties->setProperty("Hello.Endpoints", "tcp -p 10000:ssl -p 10001:udp -p 10000");
        id.properties->setProperty("Ice.Trace.Network", "2");

        _communicator = Ice::initialize(id);
        _adapter = _communicator->createObjectAdapter("Hello");
        _adapter->add(new HelloI(this), _Ice::stringToIdentity("hello"));
        _adapter->activate();
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
