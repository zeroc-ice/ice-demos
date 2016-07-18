// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Data;
using System.Windows.Threading;
using System.Windows.Input;

namespace ChatDemoGUI
{
    //
    // This class coordinates the chat client logic.
    //
    public class Coordinator : Glacier2.SessionCallback
    {
        public enum ClientState { Disconnected, Connecting, Connected, ConnectionLost, Disconnecting }

        public Coordinator(string[] args)
        {
            _app = (App)System.Windows.Application.Current;
            ObjectDataProvider odp = (ObjectDataProvider)_app.Resources["UserList"];
            _users = (UserList)odp.Data;
            odp = (ObjectDataProvider)_app.Resources["ChatModel"];
            _model = (ChatModel)odp.Data;
            _args = args;

            Ice.InitializationData initData = new Ice.InitializationData();
            initData.properties = Ice.Util.createProperties(ref _args);
            initData.dispatcher = delegate(System.Action action, Ice.Connection connection)
                {
                    if(_exit) // The GUI is being destroyed, don't use the GUI thread any more
                    {
                        action();
                    }
                    else
                    {
                        System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, action);
                    }
                };

            if(initData.properties.getProperty("Ice.Default.Router").Length == 0)
            {
                initData.properties.setProperty("IceSSL.UsePlatformCAs", "1");
                initData.properties.setProperty("IceSSL.CheckCertName", "1");
                initData.properties.setProperty("Ice.Default.Router",
                                                "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2");
            }

            _factory = new Glacier2.SessionFactoryHelper(initData, this);
        }

        public void login(LoginInfo info)
        {
            setState(ClientState.Connecting);
            _info = info;
            _session = _factory.connect(info.Username, info.Password);
        }

        public void logout()
        {
            setState(ClientState.Disconnecting);
            destroySession();
        }

        public void initEvent(string[] users)
        {
            for(int cont = 0; cont < users.Length; ++cont)
            {
                _users.Add(new User(users[cont]));
            }
        }

        //
        // Invoked from the ChatRoomCallback implementation when a user joins the chat.
        //
        public void userJoinEvent(long timestamp, string name)
        {
            _users.Add(new User(name));
            if(_chatView != null)
            {
                _chatView.appendMessage(ChatUtils.formatTimestamp(timestamp) + " - <system-message> - " + name +
                                    " joined." + Environment.NewLine);
            }
        }

        //
        // Invoked from the ChatRoomCallback implementation when a user leaves the chat.
        //
        public void userLeaveEvent(long timestamp, string name)
        {
            int index = _users.IndexOf(new User(name));
            if(index >= 0)
            {
                _users.RemoveAt(index);
                if(_chatView != null)
                {
                    _chatView.appendMessage(ChatUtils.formatTimestamp(timestamp) + " - <system-message> - " +
                                            name + " left." + Environment.NewLine);
                }
            }
        }

        //
        // Invoked from the ChatRoomCallback implementation when a user sends a message
        // to the chat.
        //
        public void userSayEvent(long timestamp, String name, String message)
        {
            if(_chatView != null)
            {
                _chatView.appendMessage(ChatUtils.formatTimestamp(timestamp) + " - <" + name + "> " +
                                        ChatUtils.unstripHtml(message) + Environment.NewLine);
            }
        }

        public void setChatView(ChatView chatView)
        {
            _chatView = chatView;
        }

        //
        //  Send a message using Ice AMI.
        //
        public async void sendMessage(string message)
        {
            if(_chat != null)
            {
                if(message.Length > _maxMessageSize)
                {
                    appendMessage("<system-message> - Message length exceeded, maximum length is " + _maxMessageSize +
                                  " characters." + Environment.NewLine);
                }
                else
                {
                    try
                    {
                        long timestamp = await _chat.sendAsync(message);
                        userSayEvent(timestamp, _username, message);
                    }
                    catch(AggregateException ae)
                    {
                        if(ae.InnerException is Chat.InvalidMessageException)
                        {
                            Chat.InvalidMessageException e = (Chat.InvalidMessageException)ae.InnerException;
                            appendMessage("<system-message> - " + e.reason + Environment.NewLine);
                        }
                        else
                        {
                            destroySession();
                        }
                    }
                }
            }
        }

        public void setFocus()
        {
            if(_model.State == ClientState.Connected && _chatView != null)
            {
                _chatView.setFocusToInput();
            }
        }

        public void setState(ClientState state)
        {
            if(state == ClientState.Disconnected)
            {
                _users.Clear();
            }

            if(state == ClientState.Connecting || state == ClientState.Disconnecting)
            {
                Mouse.OverrideCursor = Cursors.Wait;
            }
            else
            {
                Mouse.OverrideCursor = Cursors.Arrow;
            }
            _model.State = state;
        }

        public void setError(string error)
        {
            //
            // Don't display errors at that point GUI is being destroyed.
            //
            if(_exit)
            {
                return;
            }
            if(_model.State == ClientState.Connected || _model.State == ClientState.ConnectionLost)
            {
                _chatView.appendError(error);
                setState(ClientState.ConnectionLost);
            }
            else
            {
                setState(ClientState.Disconnected);
                ErrorView errorView = new ErrorView();
                errorView.ResizeMode = ResizeMode.NoResize;
                errorView.setError(error);
                ChatUtils.centerWindow(errorView,_mainView);
                errorView.ShowDialog();
            }
        }

        public string getUsername()
        {
            return _username;
        }

        public void appendMessage(string message)
        {
            if(_chatView != null)
            {
                _chatView.appendMessage(message);
            }
        }

        public void setMainView(MainView mainView)
        {
            _mainView = mainView;
        }

        public void storeWindowPrefs()
        {
            _mainView.storeWindowPrefs();
        }

        public void exit()
        {
            _exit = true;
            Ice.Communicator communicator = _session == null ? null : _session.communicator();
            destroySession();
            if(communicator != null)
            {
                communicator.waitForShutdown();
            }
        }

        #region Callback Members

        public void connectFailed(Glacier2.SessionHelper session, Exception exception)
        {
            //
            // Ignore callbacks during shutdown.
            //
            if(_exit)
            {
                return;
            }

            try
            {
                throw exception;
            }
            catch(Glacier2.CannotCreateSessionException ex)
            {
                setError("Login failed (Glacier2.CannotCreateSessionException):\n" + ex.reason);
            }
            catch(Glacier2.PermissionDeniedException ex)
            {
                setError("Login failed (Glacier2.PermissionDeniedException):\n" + ex.reason);
            }
            catch(Ice.Exception ex)
            {
                setError("Login failed (" + ex.ice_id() + ").\n" +
                         "Please check your configuration.");
            }
            catch(System.Exception ex)
            {
                setError("Login failed:\n" + ex.ToString());
            }
        }

        public async void connected(Glacier2.SessionHelper session)
        {

            //
            // Ignore callbacks during shutdown.
            //
            if(_exit)
            {
                return;
            }

            //
            // If the session has been reassigned avoid the spurious callback.
            //
            if(session != _session)
            {
                return;
            }

            Chat.ChatRoomCallbackPrx callback = Chat.ChatRoomCallbackPrxHelper.uncheckedCast(
                                _session.addWithUUID(new ChatRoomCallbackI(this)));

            _chat = Chat.ChatSessionPrxHelper.uncheckedCast(_session.session());
            try
            {
                await _chat.setCallbackAsync(callback);
                _username = ChatUtils.formatUsername(_info.Username);
                _info.save();
                setState(ClientState.Connected);
            }
            catch(Ice.CommunicatorDestroyedException)
            {
                //Ignore client session was destroyed.
            }
            catch(Exception)
            {
                destroySession();
            }
        }

        public void createdCommunicator(Glacier2.SessionHelper session)
        {
        }

        public void disconnected(Glacier2.SessionHelper session)
        {
            //
            // Ignore callbacks during shutdown.
            //
            if(_exit)
            {
                return;
            }

            _username = "";
            if(_model.State == ClientState.Disconnecting) // Connection closed by user logout/exit
            {
                setState(ClientState.Disconnected);
            }
            else if(_model.State == ClientState.Connected) // Connection lost while user was chatting
            {
                setError(
                    "<system-message> - The connection with the server was unexpectedly lost.\n" +
                    "You can try Login again from the File menu.");
            }
            else // Connection lost while user was connecting
            {
                setError(
                    "<system-message> - The connection with the server was unexpectedly lost.\n" +
                    "Try again.");
            }
        }

        #endregion

        protected void
        destroySession()
        {
            Glacier2.SessionHelper s = _session;
            _session = null;
            _chat = null;
            if(s != null)
            {
                s.destroy();
            }
        }

        private App _app = null;
        private string[] _args = null;
        private Glacier2.SessionFactoryHelper _factory;
        private LoginInfo _info = new LoginInfo();
        private Glacier2.SessionHelper _session = null;
        private Object _sessionMonitor = new Object();
        private Chat.ChatSessionPrx _chat = null;
        private ChatView _chatView = null;
        private UserList _users = null;
        private ChatModel _model = null;
        private MainView _mainView = null;

        private string _username = "";
        private const int _maxMessageSize = 1024;
        private bool _exit = false;
    }
}
