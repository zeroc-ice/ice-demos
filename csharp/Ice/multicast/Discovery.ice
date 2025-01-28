// Copyright (c) ZeroC, Inc.

#pragma once

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
