//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once
["java:package:com.zeroc.demos.android.hello"]
module Demo
{

interface Hello
{
    idempotent void sayHello(int delay);
    void shutdown();
}

}
