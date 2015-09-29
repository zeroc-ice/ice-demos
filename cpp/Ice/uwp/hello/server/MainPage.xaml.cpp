// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
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


	HelloI()
	{
	}

	virtual void
	sayHello(int delay, const Ice::Current&)
	{
		if(delay > 0)
		{
			IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
		}

		MainPage::instance()->Dispatcher->RunAsync(
			CoreDispatcherPriority::Normal, ref new DispatchedHandler([=]()
			{
				MainPage::instance()->print(ref new String(L"Hello World!\n"));
			}, CallbackContext::Any));
	}

	virtual void
	shutdown(const Ice::Current&)
	{
		Application::Current->Exit();
	}
};
}

MainPage^ MainPage::_instance = nullptr;

MainPage::MainPage()
{
    InitializeComponent();
	_instance = this;

    Ice::InitializationData id;
	id.properties = Ice::createProperties();
	id.properties->setProperty("Hello.Endpoints", "tcp -p 10000:ssl -p 10001:udp -p 10000");
	id.properties->setProperty("Ice.Trace.Network", "2");

	_communicator = Ice::initialize(id);
	_adapter = _communicator->createObjectAdapter("Hello");
	_adapter->add(new HelloI(), _communicator->stringToIdentity("hello"));
	_adapter->activate();
	print(ref new String(L"Ready to receive requests\n"));
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

MainPage^
MainPage::instance()
{
	return _instance;
}
