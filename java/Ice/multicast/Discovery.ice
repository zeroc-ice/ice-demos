//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.multicast"]
module Demo
{
    interface DiscoverReply
    {
        void reply(Object* obj);
    }

    interface Discover
    {
        void lookup(DiscoverReply* replyHere);
    }
}
