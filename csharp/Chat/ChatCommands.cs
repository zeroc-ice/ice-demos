// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;

namespace ChatDemoGUI
{

public class ChatCommands
{

    static ChatCommands()
    {
        // Initialize logout command
        _loginCommand = new RoutedUICommand("Login", "Login",typeof(ChatCommands), null);

        // Initialize logout command
        _logoutCommand = new RoutedUICommand("Logout", "Logout", typeof(ChatCommands), null);

        // Initialize exit commnad
        _exitCommand = new RoutedUICommand("Exit", "&Exit", typeof(ChatCommands), null);

        // Initialize about command
        _aboutCommand = new RoutedUICommand("About", "About", typeof(ChatCommands), null);
    }

    public static RoutedUICommand Login
    {
        get
        {
            return _loginCommand;
        }
    }

    public static RoutedUICommand Logout
    {
        get
        {
            return _logoutCommand;
        }
    }

    public static RoutedUICommand Exit
    {
        get
        {
            return _exitCommand;
        }
    }

    public static RoutedUICommand About
    {
        get
        {
            return _aboutCommand;
        }
    }

    private static RoutedUICommand _loginCommand;
    private static RoutedUICommand _logoutCommand;
    private static RoutedUICommand _exitCommand;
    private static RoutedUICommand _aboutCommand;
}

}
