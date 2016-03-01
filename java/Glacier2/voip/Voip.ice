// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Glacier2/Session.ice>

module Voip
{

interface Control
{
    void incomingCall();
};

interface Session extends Glacier2::Session
{
    void setControl(Control* ctrl);

    void simulateCall(int delay);

    void refresh();
};

};
