// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.library.controller;

interface SessionAdapter
{
    void destroy();
    Demo.LibraryPrx getLibrary();
}
