// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef PATCH_CLIENT_H
#define PATCH_CLIENT_H

#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"

class CPatchClientApp : public CWinApp
{
public:
    CPatchClientApp();

    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CPatchClientApp theApp;

#endif
