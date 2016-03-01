// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;

namespace ChatDemoGUI
{
    public partial class MainView : Window
    {
        public MainView()
        {
            InitializeComponent();
            App app = (App)System.Windows.Application.Current;
            ObjectDataProvider odp = (ObjectDataProvider)app.Resources["ChatModel"];
            app.getCoordinator().setMainView(this);
            _model = (ChatModel)odp.Data;

            // Disable the browse back command
            NavigationCommands.BrowseBack.InputGestures.Clear();
            if(!loadWindowsPrefs())
            {
                Width = 400;
                Height = 540;
                ChatUtils.locateOnScreen(this);
            }
        }

        private void windowActivated(object sender,EventArgs e)
        {
            ((App)System.Windows.Application.Current).getCoordinator().setFocus();
        }

        private void login(object sender, ExecutedRoutedEventArgs args)
        {
            ((App)System.Windows.Application.Current).getCoordinator().setState(Coordinator.ClientState.Disconnected);
        }

        private void logout(object sender,ExecutedRoutedEventArgs args)
        {
            ((App)System.Windows.Application.Current).getCoordinator().logout();
        }

        private void exit(object sender,ExecutedRoutedEventArgs args)
        {
            System.Windows.Application.Current.Shutdown(0);
        }

        private void isLogoutEnabled(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = _model.State == Coordinator.ClientState.Connected;
        }

        private void about(object sender,ExecutedRoutedEventArgs args)
        {
            String aboutMessage = "Chat Demo \n"
                                 + "Copyright \u00A9 2005-2016 ZeroC, Inc. All rights reserved.\n";
            string caption = "Chat Demo - About";
            MessageBoxImage icon = MessageBoxImage.Information;
            MessageBoxButton button = MessageBoxButton.OK;
            MessageBox.Show(aboutMessage, caption, button, icon);
        }

        private bool loadWindowsPrefs()
        {
            if(Properties.Settings.Default.WindowPrefs)
            {
                Width = Properties.Settings.Default.Width;
                Height = Properties.Settings.Default.Height;
                Top = Properties.Settings.Default.Top;
                Left = Properties.Settings.Default.Left;
            }
            return Properties.Settings.Default.WindowPrefs;
        }

        public void storeWindowPrefs()
        {
            Properties.Settings.Default.Width = Width;
            Properties.Settings.Default.Height = Height;
            Properties.Settings.Default.Top = Top;
            Properties.Settings.Default.Left = Left;
            Properties.Settings.Default.WindowPrefs = true;
            Properties.Settings.Default.Save();
        }

        private ChatModel _model;

        private void isLoginEnabled(object sender,CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = _model.State == Coordinator.ClientState.ConnectionLost;
        }
    }
}
