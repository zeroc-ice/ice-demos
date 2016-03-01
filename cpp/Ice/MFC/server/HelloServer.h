// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#ifndef HELLO_SERVER_H
#define HELLO_SERVER_H

#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"

class CHelloServerApp : public CWinApp
{
public:
    CHelloServerApp();

    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CHelloServerApp theApp;

#endif
