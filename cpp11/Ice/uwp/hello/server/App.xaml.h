
#pragma once

#include "App.g.h"

namespace hello
{

ref class App sealed
{
public:

    App();
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^) override;
};

}
