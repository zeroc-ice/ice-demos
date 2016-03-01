// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;
using System.Windows.Data;
using Microsoft.Win32;

namespace ChatDemoGUI
{
    //
    //  The App class is a partial class that is partially defined in App.xaml.
    //
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            if(_hasNetFramework)
            {
                _coordinator.storeWindowPrefs();
                _coordinator.exit();
            }
        }

        public Coordinator getCoordinator()
        {
            return _coordinator;
        }

        private void AppStartup(object sender, StartupEventArgs e)
        {
            PlatformID id = Environment.OSVersion.Platform;
            bool windows = id == PlatformID.Win32NT || id  == PlatformID.Win32S || id == PlatformID.Win32Windows ||
                           id == PlatformID.WinCE;
            if(windows)
            {
                RegistryKey netKey = Registry.LocalMachine.OpenSubKey(
                                                        "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Client");
                if(netKey != null)
                {
                    _hasNetFramework = (int)netKey.GetValue("Install") == 1;
                }

                if(!_hasNetFramework)
                {
                    netKey = Registry.LocalMachine.OpenSubKey(
                                                        "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full");
                    if(netKey != null)
                    {
                        _hasNetFramework = (int)netKey.GetValue("Install") == 1;
                    }
                }

                if(!_hasNetFramework)
                {
                    netKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.5");
                    if(netKey != null)
                    {
                        _hasNetFramework = (int)netKey.GetValue("Install") == 1;
                    }
                }
            }

            if(_hasNetFramework)
            {
                _coordinator = new Coordinator(e.Args);
            }
            else
            {
                string errorMessage = "Chat Demo \n" +
                    "You need to install Microsoft .NET Framework v3.5 or higher on a Windows OS in order to run " +
                    "this application.\n" +
                    "Refer to the README file in your Chat Demo distribution for more information.";
                string caption = "Chat Demo - Error";
                MessageBoxImage icon = MessageBoxImage.Error;
                MessageBoxButton button = MessageBoxButton.OK;
                MessageBox.Show(errorMessage, caption, button, icon);
                Shutdown(-1);
            }
        }

        private Coordinator _coordinator = null;
        private bool _hasNetFramework = false;
    }
}
