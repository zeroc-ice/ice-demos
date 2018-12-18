// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include "pch.h"
#include "MainPage.xaml.h"

using namespace bidir;

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
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs)
{
    // Do not repeat app initialization when already running, just ensure that
    // the window is active
    if (pArgs->PreviousExecutionState == ApplicationExecutionState::Running)
    {
        Window::Current->Activate();
        return;
    }

    // Create a Frame to act navigation context and navigate to the first page
    auto rootFrame = ref new Frame();
    if (!rootFrame->Navigate(TypeName(MainPage::typeid)))
    {
        throw ref new FailureException("Failed to create initial page");
    }

    // Place the frame in the current Window and ensure that it is active
    Window::Current->Content = rootFrame;
    Window::Current->Activate();
}
