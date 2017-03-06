//
// ChatViewPage.xaml.h
// Declaration of the ChatViewPage class
//

#pragma once

#include "ChatViewPage.g.h"

namespace simpleChat
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ChatViewPage sealed
	{
	public:
		ChatViewPage();
		void setError(Platform::String^ err);
		void appendMessage(Platform::String^ message);
		void inputKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
	};
}
