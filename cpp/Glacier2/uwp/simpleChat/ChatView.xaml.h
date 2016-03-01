// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "ChatView.g.h"

namespace simpleChat
{

public ref class ChatView sealed
{
public:
        
    ChatView();
    void setError(Platform::String^ err);
    void appendMessage(Platform::String^ message);
        
private:
        
    void inputKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
};

}
