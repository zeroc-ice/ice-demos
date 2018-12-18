// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "MainPage.g.h"
#include <Ice/Ice.h>

namespace hello
{

public ref class MainPage sealed
{
public:

    MainPage();
    void resume();
    void print(Platform::String^ message);

private:

    void clear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

    Ice::CommunicatorHolder _communicator;
};

}
