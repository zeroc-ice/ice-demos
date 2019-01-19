//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.library.controller;

import com.zeroc.demos.android.library.Demo.*;

interface SessionAdapter
{
    void destroy();
    LibraryPrx getLibrary();
}
