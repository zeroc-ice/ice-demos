// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ChatDemoGUI
{
    //
    //
    // Implementation of the ChatRoomCallback servant.
    //
    // The implementation delegates to the methods of the Coordinator object
    // to communicate with the UI.
    //
    public class ChatRoomCallbackI : Chat.ChatRoomCallbackDisp_
    {
        public ChatRoomCallbackI(Coordinator coordinator)
        {
            _coordinator = coordinator;
        }

        public override void init(string[] users, Ice.Current currrent)
        {
            _coordinator.initEvent(users);
        }

        public override void send(long timestamp, string name, string message, Ice.Current current)
        {
            if(!name.Equals(_coordinator.getUsername(), StringComparison.CurrentCultureIgnoreCase))
            {
                _coordinator.userSayEvent(timestamp, name, message);
            }
        }

        public override void join(long timestamp, string name, Ice.Current current)
        {
            _coordinator.userJoinEvent(timestamp, name);
        }

        public override void leave(long timestamp, string name, Ice.Current current)
        {
            _coordinator.userLeaveEvent(timestamp, name);
        }

        private Coordinator _coordinator;
    }
}
