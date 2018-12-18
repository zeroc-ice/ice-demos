// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "App.g.h"

namespace bidir
{

ref class App sealed
{
public:
    App();
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^) override;
    void enteredBackground(Platform::Object^, Windows::ApplicationModel::EnteredBackgroundEventArgs^);
    void leavingBackground(Platform::Object^, Windows::ApplicationModel::LeavingBackgroundEventArgs^);
};

}
