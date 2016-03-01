// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Data;
using System.Globalization;

namespace ChatDemoGUI
{
    class ChatUtils
    {
        /**
         *
         * This method removes HTML escape sequences from a string
         * and returns a copy of the string.
         *
         **/
        public static string unstripHtml(string data)
        {
            data = data.Replace("&quot", "\"");
            data = data.Replace("&#39", "'");
            data = data.Replace("&lt;", "<");
            data = data.Replace("&gt;", ">");
            data = data.Replace("&amp;", "&");
            return data;
        }

        public static string formatUsername(string name)
        {
            try
            {
                return name.Substring(0, 1).ToUpper() + name.Substring(1, name.Length - 1).ToLower();
            }
            catch (ArgumentOutOfRangeException)
            {
            }
            return name;
        }

        public static string getSystemUsername()
        {
            string username = System.Security.Principal.WindowsIdentity.GetCurrent().Name.ToString();
            int pos = username.IndexOf("\\");
            if(pos > 0)
            {
                username = username.Substring(pos + 1, username.Length - pos -1);
            }
            return formatUsername(username);
        }

        public static string formatTimestamp(long timestamp)
        {
            DateTime date = new System.DateTime(1970, 1, 1, 0, 0, 0, 0);
            date = date.AddMilliseconds(timestamp);
            return date.ToLocalTime().ToString("T");
        }

        static public void locateOnScreen(System.Windows.Window window)
        {
            window.Left = (System.Windows.SystemParameters.PrimaryScreenWidth - window.Width) / 2;
            window.Top = (System.Windows.SystemParameters.PrimaryScreenHeight - window.Height) / 2;
        }

        static public void centerWindow(System.Windows.Window w1, System.Windows.Window w)
        {
           w1.Top = w.Top + ((w.Height - w1.Height) / 2);
           w1.Left = w.Left + ((w.Width - w1.Width) / 2);
        }
    }

    //
    // ValueConverters for use with XAML bindings.
    //
    [ValueConversion(typeof(Coordinator.ClientState),typeof(string))]
    class StatusConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object paramenter, CultureInfo culture)
        {
            Coordinator.ClientState state = (Coordinator.ClientState)value;
            if(state == Coordinator.ClientState.Connected)
            {
                return "Online";
            }
            else if(state == Coordinator.ClientState.Connecting)
            {
                return "Connecting";
            }
            else if(state == Coordinator.ClientState.Disconnecting)
            {
                return "Disconnecting";
            }
            else
            {
                return "Offline";
            }
        }

        public object ConvertBack(object value, Type targetType, object paramenter, CultureInfo culture)
        {
            string status = (string)value;
            if(status == "Online")
            {
                return Coordinator.ClientState.Connected;
            }
            else if(status == "Connecting")
            {
                return Coordinator.ClientState.Connecting;
            }
            else if(status == "Disconnecting")
            {
                return Coordinator.ClientState.Disconnecting;
            }
            else
            {
                return Coordinator.ClientState.Disconnected;
            }
        }
    }

    [ValueConversion(typeof(Coordinator.ClientState), typeof(bool))]
    class ConnectingBoolConverter : IValueConverter
    {
        public object Convert(object value,Type targetType,object paramenter,CultureInfo culture)
        {
            Coordinator.ClientState _value = (Coordinator.ClientState)value;
            if(_value == Coordinator.ClientState.Connecting)
            {
                return false;
            }
            return true;
        }

        public object ConvertBack(object value,Type targetType,object paramenter,CultureInfo culture)
        {
            bool _value = (bool)value;
            if(_value == true)
            {
                return Coordinator.ClientState.Connecting;
            }
            return Coordinator.ClientState.Disconnected;
        }
    }

    [ValueConversion(typeof(bool), typeof(Visibility))]
    class BoolVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object paramenter, CultureInfo culture)
        {
            bool _value = (bool)value;
            if (_value)
            {
                return Visibility.Visible;
            }
            else
            {
                return Visibility.Collapsed;
            }
        }

        public object ConvertBack(object value, Type targetType, object paramenter, CultureInfo culture)
        {
            Visibility _value = (Visibility)value;
            if (Visibility.Visible == _value)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //
    // This class implements the IValueConverter interface to transform a bool value to its
    // inverse. We use this in our xaml definitions for binding to the inverse value of
    // a boolean property.
    //
    [ValueConversion(typeof(bool), typeof(bool))]
    class BoolOpositeConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object paramenter, CultureInfo culture)
        {
            bool _value = (bool)value;
            return !_value;
        }

        public object ConvertBack(object value, Type targetType, object paramenter, CultureInfo culture)
        {
            bool _value = (bool)value;
            return !_value;
        }
    }
}
