// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{
    interface Nested
    {
        void nestedCall(int level, Nested* proxy);
    }
}
