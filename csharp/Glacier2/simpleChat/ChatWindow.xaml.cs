// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;

using Demo;

namespace Glacier2.simpleChat.client
{
    public class LoginData
    {
        public LoginData()
        {
            routerHost = "127.0.0.1";
            userName = "test";
            password = "";
        }

        public string routerHost;
        public string userName;
        public string password;
    }

    class Util
    {
        static public void
        locateOnScreen(Window window)
        {
            window.Left = (SystemParameters.PrimaryScreenWidth - window.Width) / 2;
            window.Top = (SystemParameters.PrimaryScreenHeight - window.Height) / 2;
        }

        static public void
        centerWindow(Window w1, Window w)
        {
            w1.Top = w.Top + ((w.Height - w1.Height) / 2);
            w1.Left = w.Left + ((w.Width - w1.Width) / 2);
        }
    }

    /// <summary>
    /// Interaction logic for ChatWindow.xaml
    /// </summary>
    public partial class ChatWindow : Window, SessionCallback
    {
        private class ChatCallbackI : ChatCallbackDisp_
        {
            public ChatCallbackI(ChatWindow window)
            {
                _window = window;
            }

            public override void
            message(string data, Ice.Current current)
            {
                _window.appendMessage(data + Environment.NewLine);
            }

            private ChatWindow _window;
        }

        public ChatWindow()
        {
            Ice.InitializationData initData = new Ice.InitializationData();

            initData.properties = Ice.Util.createProperties();
            initData.properties.load("config.client");

            // Dispatch servant calls and AMI callbacks with this windows Dispatcher.
            initData.dispatcher = (System.Action action, Ice.Connection connection) =>
            {
                Dispatcher.BeginInvoke(DispatcherPriority.Normal, action);
            };

            _factory = new SessionFactoryHelper(initData, this);
            InitializeComponent();
            Util.locateOnScreen(this);
        }

        private void
        login(object sender, ExecutedRoutedEventArgs args)
        {
            doLogin();
        }

        public void doLogin()
        {
            LoginDialog loginDialog = new LoginDialog(_loginData);
            Util.centerWindow(loginDialog, this);
            if(loginDialog.ShowModal())
            {
                status.Content = "Connecting";
                _factory.setRouterHost(_loginData.routerHost);
                _factory.setRouterIdentity(new Ice.Identity("router", "DemoGlacier2"));
                _session = _factory.connect(_loginData.userName, _loginData.password);

                _cancelDialog = new CancelDialog();
                Util.centerWindow(_cancelDialog, this);
                if(_cancelDialog.ShowModal())
                {
                    destroySession();
                }
            }
        }

        private void
        logout(object sender, ExecutedRoutedEventArgs args)
        {
            txtMessages.Text = "";
            status.Content = "Logging out";
            destroySession();
        }

        private void
        exit(object sender, ExecutedRoutedEventArgs args)
        {
            Close();
        }

        private void
        windowClosed(object sender, EventArgs e)
        {
            lock(this)
            {
                destroySession();
            }
            App.Current.Shutdown(0);
        }

        private void
        isLogoutEnabled(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = _session != null && _session.isConnected();
        }

        private void
        isLoginEnabled(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = _session == null || !_session.isConnected();
        }

        //
        // Event handler attached to txtChatImputLine onKeyDown.
        // If the key is the Enter key, it sends the message asynchronously
        // and cleans the input line; otherwise, it does nothing.
        //
        private void
        sendMessage(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                sendMessageAsync(input.Text.Trim());
                input.Text = "";
            }
        }

        private async void
        sendMessageAsync(string message)
        {
            if(!string.IsNullOrEmpty(message))
            {
                try
                {
                    await _chat.sayAsync(message);
                }
                catch(AggregateException ex)
                {
                    appendMessage("<system-message> - " + ex.ToString() + Environment.NewLine);
                }
            }
        }

        private void
        scrollDown(object sender, SizeChangedEventArgs e)
        {
            txtMessages.ScrollToEnd();
        }

        private void
        closeCancelDialog()
        {
            if(_cancelDialog != null)
            {
                _cancelDialog.Close();
                _cancelDialog = null;
            }
        }

        public void
        appendMessage(string message)
        {
            txtMessages.AppendText(message);
            txtMessages.ScrollToEnd();
        }

        private void
        destroySession()
        {
            if(_session != null)
            {
                _session.destroy();
                _session = null;
            }
        }

        private LoginData _loginData = new LoginData();
        private CancelDialog _cancelDialog = new CancelDialog();
        private SessionFactoryHelper _factory;
        private SessionHelper _session;
        private ChatSessionPrx _chat;

        #region Callback Members

        public void connectFailed(SessionHelper session, Exception ex)
        {
            // If the session has been reassigned avoid the
            // spurious callback.
            if(session != _session)
            {
                return;
            }

            closeCancelDialog();
            status.Content = ex.GetType();
        }

        public async void connected(SessionHelper session)
        {
            // If the session has been reassigned avoid the
            // spurious callback.
            if(session != _session)
            {
                return;
            }

            Ice.Object servant = new ChatCallbackI(this);

            ChatCallbackPrx callback = Demo.ChatCallbackPrxHelper.uncheckedCast(_session.addWithUUID(servant));
            _chat = ChatSessionPrxHelper.uncheckedCast(_session.session());
            try
            {
                await _chat.setCallbackAsync(callback);
                closeCancelDialog();
                input.IsEnabled = true;
                status.Content = "Connected with " + _loginData.routerHost;
            }
            catch(Exception)
            {
                if(_session != null)
                {
                    _session.destroy();
                }
            }
        }

        public void createdCommunicator(SessionHelper session)
        {
        }

        public void disconnected(SessionHelper session)
        {
            // If the session has been reassigned avoid the
            // spurious callback.
            if(session != _session)
            {
                return;
            }
            closeCancelDialog();
            _session = null;
            _chat = null;
            input.IsEnabled = false;
            status.Content = "Not connected";
        }

        #endregion
    }
}
