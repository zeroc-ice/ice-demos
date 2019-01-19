//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.demos.Chat;

//
// This class implements the ChatRoomCallback interface.
//
class ChatRoomCallbackI implements ChatRoomCallback
{
    public ChatRoomCallbackI(Coordinator coordinator)
    {
        _coordinator = coordinator;
    }

    public void init(String[] users, com.zeroc.Ice.Current currrent)
    {
        _coordinator.initEvent(users);
    }

    public void send(long timestamp, String name, String message, com.zeroc.Ice.Current currrent)
    {
        if(name.compareToIgnoreCase(_coordinator.getUsername()) != 0)
        {
            _coordinator.userSayEvent(timestamp, name, message);
        }
    }

    public void join(long timestamp, String name, com.zeroc.Ice.Current currrent)
    {
        _coordinator.userJoinEvent(timestamp, name);
    }

    public void leave(long timestamp, String name, com.zeroc.Ice.Current currrent)
    {
        _coordinator.userLeaveEvent(timestamp, name);
    }

    private final Coordinator _coordinator;
}
