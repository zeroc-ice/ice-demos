// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "App.g.h"

namespace hello
{

ref class App sealed
{
public:

    App();
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^) override;
    void leavingBackground(Platform::Object^, Windows::ApplicationModel::LeavingBackgroundEventArgs^);
};

}
