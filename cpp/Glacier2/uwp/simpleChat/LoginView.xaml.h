// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "LoginView.g.h"

namespace simpleChat
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class LoginView sealed
{
public:

    LoginView();
    void setError(Platform::String^ msg);
    void signinCompleted()
    {
        signin->IsEnabled = true;
    }

protected:

    virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

private:

    void signinClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};

}
