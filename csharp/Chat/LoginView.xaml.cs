// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace ChatDemoGUI
{
    public partial class LoginView : Page
    {
        public LoginView()
        {
            InitializeComponent();
            App app = (App)System.Windows.Application.Current;
            ObjectDataProvider odp = (ObjectDataProvider)app.Resources["ChatModel"];
            _model = (ChatModel)odp.Data;
            _coordinator = app.getCoordinator();
        }

        private void login(object sender, RoutedEventArgs e)
        {
            LoginInfo info = new LoginInfo();
            info.Username = _model.LoginData.Username;
            info.Password = txtPassword.Password;
            _coordinator.login(info);
        }

        private void pageLoaded(object sender, RoutedEventArgs e)
        {
            Keyboard.Focus(txtUsername);
        }

        private ChatModel _model = null;
        private Coordinator _coordinator = null;
    }
}
