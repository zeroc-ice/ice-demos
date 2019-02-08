//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.nested"]
module Demo
{
    interface Nested
    {
        void nestedCall(int level, Nested* proxy);
    }
}
