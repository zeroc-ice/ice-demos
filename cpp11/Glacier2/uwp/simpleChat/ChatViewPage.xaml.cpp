//
// ChatView.xaml.cpp
// Implementation of the ChatView class
//

#include "pch.h"
#include "ChatViewPage.xaml.h"
#include "MainPage.xaml.h"

#include <Glacier2/Glacier2.h>
#include <Chat.h>
#include <string>

using namespace simpleChat;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace std;

ChatViewPage::ChatViewPage()
{
	InitializeComponent();
	MainPage::instance()->_chatView = this;
}

void
ChatViewPage::setError(String^ err)
{
	appendMessage(L"<system-message> " + err);
}

void
ChatViewPage::appendMessage(String^ message)
{
	messages->Text += message + L"\n";
	messages->UpdateLayout();
#if (_WIN32_WINNT > 0x0602)
	Scroller->ChangeView(nullptr, Scroller->ScrollableHeight, nullptr);
#else
	Scroller->ScrollToVerticalOffset(Scroller->ScrollableHeight);
#endif
}

void
ChatViewPage::inputKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	if (e->Key == Windows::System::VirtualKey::Enter && !input->Text->IsEmpty())
	{
		string msg = Ice::wstringToString(input->Text->Data());
		input->Text = "";
		MainPage::instance()->coordinator()->say(msg);
	}
}
