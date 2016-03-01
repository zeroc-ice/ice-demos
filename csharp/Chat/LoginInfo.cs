// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;

namespace ChatDemoGUI
{
    //
    // This class stores information needed to log in. All info other
    // than the password is stored in Properties.Settings.
    //
    public class LoginInfo
    {
        public LoginInfo()
        {
            _userName = ChatUtils.getSystemUsername();
            load();
        }

        public void save()
        {
            Properties.Settings.Default.Username = Username;
            Properties.Settings.Default.Host = Host;
            Properties.Settings.Default.Save();
        }

        public void load()
        {
            if(Properties.Settings.Default.Username != "")
            {
                Username = Properties.Settings.Default.Username;
            }
            Host = Properties.Settings.Default.Host;
        }

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
        private string _host = "127.0.0.1";
    }
}
