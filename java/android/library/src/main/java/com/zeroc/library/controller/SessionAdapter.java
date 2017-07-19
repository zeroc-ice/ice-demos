// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.library.controller;

interface SessionAdapter
{
    void destroy();
    Demo.LibraryPrx getLibrary();
}
