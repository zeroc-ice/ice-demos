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
CallbackSenderI::addClient(const Ice::Identity& ident, const Ice::Current& current)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	ostringstream os;
	os << "adding client `";
	os << Ice::identityToString(ident);
	os << "'\n";
	_page->print(os.str());

	CallbackReceiverPrx client = CallbackReceiverPrx::uncheckedCast(current.con->createProxy(ident));
	_clients.insert(client);
}

void
CallbackSenderI::run()
{
	int num = 0;
	while(num >= 0)
	{
		std::set<Demo::CallbackReceiverPrx> clients;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
			timedWait(IceUtil::Time::seconds(2));

			clients = _clients;
		}

		if(!clients.empty())
		{
			++num;
			for(set<CallbackReceiverPrx>::iterator p = clients.begin(); p != clients.end(); ++p)
			{
				try
				{
					(*p)->callback(num);
				}
				catch(const Ice::Exception& ex)
				{
					ostringstream os;
					os << "removing client `";
					os << Ice::identityToString((*p)->ice_getIdentity());
					os << "':\n";
					os << ex;
					os << "\n";
					_page->print(os.str());

					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
					_clients.erase(*p);
				}
			}
		}
	}
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
        CallbackSenderIPtr sender = new CallbackSenderI(this, _communicator);
        _adapter->add(sender, _Ice::stringToIdentity("sender"));
        _adapter->activate();

        sender->start();
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
#if (_WIN32_WINNT > 0x0602)
									scroller->ChangeView(nullptr, scroller->ScrollableHeight, nullptr);
#else
									scroller->ScrollToVerticalOffset(scroller->ScrollableHeight);
#endif
								}, 
							CallbackContext::Any));
}
