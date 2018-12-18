// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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
            _coordinator.storeWindowPrefs();
            _coordinator.exit();
        }

        public Coordinator getCoordinator()
        {
            return _coordinator;
        }

        private void AppStartup(object sender, StartupEventArgs e)
        {
            _coordinator = new Coordinator(e.Args);
        }

        private Coordinator _coordinator = null;
    }
}
