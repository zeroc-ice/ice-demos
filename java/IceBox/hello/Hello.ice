// Copyright (c) ZeroC, Inc.

#pragma once

["java:package:com.zeroc.demos.IceBox.hello"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello();
    }
}
