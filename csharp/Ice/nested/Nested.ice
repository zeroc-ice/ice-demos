// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    interface Nested
    {
        void nestedCall(int level, Nested* proxy);
    }
}
