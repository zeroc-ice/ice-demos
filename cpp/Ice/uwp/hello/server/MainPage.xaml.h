// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "MainPage.g.h"
#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <Hello.h>

namespace hello
{

public ref class MainPage sealed
{
public:

    MainPage();

	void print(Platform::String^ message);

private:

    void clear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

    Ice::CommunicatorPtr _communicator;
	Ice::ObjectAdapterPtr _adapter;
};

}
