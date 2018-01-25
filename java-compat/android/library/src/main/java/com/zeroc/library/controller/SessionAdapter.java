// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.library.controller;

interface SessionAdapter
{
    void destroy();
    void refresh();
    Demo.LibraryPrx getLibrary();
}
