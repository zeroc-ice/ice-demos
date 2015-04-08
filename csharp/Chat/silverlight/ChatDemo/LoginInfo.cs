// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
//
// **********************************************************************

using System;

namespace ChatDemo
{
    //
    // This class stores information needed to log in. All info other
    // than the password is stored in Properties.Settings.
    //
    public class LoginInfo
    {
        public string Username
        {
            set
            {
                _userName = value;
            }
            get
            {
                return _userName;
            }
        }

        public string Password
        {
            set
            {
                _userPassword = value;
            }
            get
            {
                return _userPassword;
            }
        }

        public string Host
        {
            set
            {
                _host = value;
            }
            get
            {
                return _host;
            }
        }

        private string _userName = "";
        private string _userPassword = "";
        private string _host = "demo.zeroc.com";
    }
}
