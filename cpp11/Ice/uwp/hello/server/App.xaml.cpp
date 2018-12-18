// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"

using namespace hello;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

App::App()
{
    InitializeComponent();
    this->LeavingBackground += ref new LeavingBackgroundEventHandler(this, &App::leavingBackground);
}

void
App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
    if(rootFrame == nullptr)
    {
        rootFrame = ref new Frame();
        if(rootFrame->Content == nullptr)
        {
            rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
        }
        Window::Current->Content = rootFrame;
        Window::Current->Activate();
    }
    else
    {
        if(rootFrame->Content == nullptr)
        {
            rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
        }
        Window::Current->Activate();
    }
}

void App::leavingBackground(Platform::Object^ sender, LeavingBackgroundEventArgs^ e)
{
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
    if(rootFrame != nullptr)
    {
        auto mainPage = dynamic_cast<MainPage^>(rootFrame->Content);
        if(mainPage != nullptr)
        {
            mainPage->resume();
        }
    }
}
