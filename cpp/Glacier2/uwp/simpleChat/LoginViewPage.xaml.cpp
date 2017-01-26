// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "LoginViewPage.xaml.h"
#include "MainPage.xaml.h"

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

LoginViewPage::LoginViewPage()
{
    InitializeComponent();
    MainPage::instance()->_loginView = this;
}

void
LoginViewPage::setError(String^ err)
{
    signin->IsEnabled = true;
    errorBorder->Visibility = Windows::UI::Xaml::Visibility::Visible;
    error->Text = err;
}

void 
LoginViewPage::OnNavigatedTo(NavigationEventArgs^)
{
    LoginData loginData = MainPage::instance()->coordinator()->loginData();
    if(!loginData.hostname.empty())
    {
        hostname->Text = ref new String(Ice::stringToWstring(loginData.hostname).c_str());
    }
    if(!loginData.username.empty())
    {
        username->Text = ref new String(Ice::stringToWstring(loginData.username).c_str());
    }
    if(!loginData.password.empty())
    {
        password->Password = ref new String(Ice::stringToWstring(loginData.password).c_str());
    }
}

void 
LoginViewPage::signinClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    LoginData loginData;
    loginData.hostname = Ice::wstringToString(hostname->Text->Data());
    loginData.username = Ice::wstringToString(username->Text->Data());
    loginData.password = Ice::wstringToString(password->Password->Data());
    signin->IsEnabled = false;
    error->Text = "";
    errorBorder->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
    MainPage::instance()->coordinator()->signIn(loginData);
}
