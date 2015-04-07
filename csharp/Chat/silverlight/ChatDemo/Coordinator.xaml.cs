// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Browser;
using System.Windows.Threading;

namespace ChatDemo
{
    public enum ClientState { Disconnected, Connecting, Connected, ConnectionLost, Disconnecting }

    public class ChatRoomCallbackI : Chat.ChatRoomCallbackDisp_
    {
        public ChatRoomCallbackI(Coordinator coordinator)
        {
            _coordinator = coordinator;
        }

        public override void init(string[] users, Ice.Current currrent)
        {
            _coordinator.initEvent(users);
        }

        public override void send(long timestamp, string name, string message, Ice.Current current)
        {
            if(!name.Equals(_coordinator.getUsername(), StringComparison.CurrentCultureIgnoreCase))
            {
                _coordinator.userSayEvent(timestamp, name, message);
            }
        }

        public override void join(long timestamp, string name, Ice.Current current)
        {
            _coordinator.userJoinEvent(timestamp, name);
        }

        public override void leave(long timestamp, string name, Ice.Current current)
        {
            _coordinator.userLeaveEvent(timestamp, name);
        }

        private Coordinator _coordinator;
    }

    public class AMI_ChatSession_sendI
    {
        public AMI_ChatSession_sendI(Coordinator coordinator, string message)
        {
            _coordinator = coordinator;
            _message = message;
        }

        public void sendMessageResponse(long timestamp)
        {
            _coordinator.sendMessageResponse(timestamp, ChatUtils.stripHtml(_message));
        }

        private Coordinator _coordinator;
        private string _message;
    }

    //
    // The Coordinator class controls the logic of the demo. It's a ScriptableType
    // class, which means that parts of the class can be accessed from JavaScript code
    // using the Silverlight plugin. 
    // 
    // This is a partial class that is partially implemented in the Coordinator.xaml file.
    //
    [ScriptableType]
    public partial class Coordinator : UserControl, Glacier2.SessionCallback
    {
        public Coordinator()
        {
            InitializeComponent();
            //
            // Register our coordinator with the scripting engine. This is needed to
            // call scriptable methods from the JavaScript Silverlight control.
            //
            HtmlPage.RegisterScriptableObject("ChatCoordinator", this);
        }

        public void Page_Loaded(object o, EventArgs e)
        {
            _chatView = HtmlPage.Window.CreateInstance("ChatViewProxy");
            HtmlPage.Window.Invoke("initialize");
        }

        //
        // This method is scriptable and can be called from JavaScript.
        //
        [ScriptableMember]
        public void setDefaultRouter(string defaultRouter)
        {
            _defaultRouter = defaultRouter;
        }

        [ScriptableMember]
        public void setClientAccessPolicyProtocol(string policyProtocol)
        {
            _policyProtocol = policyProtocol;
        }

        //
        // This method is scriptable and can be called from JavaScript.
        //
        [ScriptableMember]
        public void login(string name, string password)
        {
            setState(ClientState.Connecting);

            try
            {
                _username = ChatUtils.formatUsername(name);

                //
                // Configure properties needed by Ice for Silverlight.
                //
                Ice.InitializationData initData = new Ice.InitializationData();
                initData.properties = Ice.Util.createProperties();
                if(_policyProtocol.Equals("Http"))
                {
                    initData.properties.setProperty("Ice.ClientAccessPolicyProtocol", "Http");
                }
                initData.properties.setProperty("Ice.FactoryAssemblies", "ChatDemo,version=1.4.0.0");
                initData.dispatcher = delegate(System.Action action, Ice.Connection connection)
                {
                    Dispatcher.BeginInvoke(action);
                };

                Glacier2.SessionFactoryHelper factory = new Glacier2.SessionFactoryHelper(initData, this);
                factory.setRouterHost(_defaultRouter);
                _session = factory.connect(name, password);
            }
            catch(Ice.LocalException ex)
            {
                _chatView.Invoke("setError", "<div>" + ex.ToString() + "</div>");
            }
        }

        //
        // Glacier2.SessionCallback interface.
        //
        public void createdCommunicator(Glacier2.SessionHelper session)
        {
        }

        public void connectFailed(Glacier2.SessionHelper session, Exception ex)
        {
            // If the session has been reassigned avoid the
            // spurious callback.
            if(session != _session)
            {
                return;
            }

            if(ex is Glacier2.CannotCreateSessionException)
            {
                _chatView.Invoke("setError", "Login failed (Glacier2.CannotCreateSessionException):\n" +
                                             ((Glacier2.CannotCreateSessionException)(ex)).reason);
            }
            else if(ex is Glacier2.PermissionDeniedException)
            {
                _chatView.Invoke("setError", "Login failed (Glacier2.PermissionDeniedException):\n" +
                                             ((Glacier2.PermissionDeniedException)(ex)).reason);
            }
            else if(ex is Ice.Exception)
            {
                _chatView.Invoke("setError",
                                 "Login failed (" + ((Ice.Exception)(ex)).ice_name() + ").\n" +
                                 "Please check your server:\n" +
                                 "a Glacier2 router should be running on " + _defaultRouter + " and\n" +
                                 "the Ice for Silverlight policy server should be running on the same " +
                                 "address the Glacier2 router is running.");
            }
            else
            {
                _chatView.Invoke("setError", "Login failed:\n" + ex.ToString());
            }

        }

        public void connected(Glacier2.SessionHelper session)
        {
            // If the session has been reassigned avoid the
            // spurious callback.
            if(session != _session)
            {
                return;
            }

            Chat.ChatRoomCallbackPrx callback = Chat.ChatRoomCallbackPrxHelper.uncheckedCast(
                                _session.addWithUUID(new ChatRoomCallbackI(this)));

            _chat = Chat.ChatSessionPrxHelper.uncheckedCast(_session.session());

            try
            {
                _chat.begin_setCallback(callback).whenCompleted(
                    delegate()
                    {
                        setState(ClientState.Connected);
                    },
                    delegate(Ice.Exception ex)
                    {
                        _session.destroy();
                    });
            }
            catch(Ice.CommunicatorDestroyedException)
            {
                //Ignore client session was destroyed.
            }
        }

        public void disconnected(Glacier2.SessionHelper session)
        {
            // If the session has been reassigned avoid the
            // spurious callback.
            if(session != _session)
            {
                return;
            }

            _username = "";

            if(_state == ClientState.Disconnecting) // Connection closed by user logout/exit
            {
                _chatView.Invoke("setState", "Disconnected");
            }
            else if(_state == ClientState.Connected) // Connection lost while user was chatting
            {
                _chatView.Invoke("setError",
                                 "<system-message> - The connection with the server was unexpectedly lost.\n" +
                                 "You can try Login again from the File menu.");
            }
            else // Connection lost while user was connecting
            {
                _chatView.Invoke("setError",
                                 "<system-message> - The connection with the server was unexpectedly lost.\n" +
                                 "Try again.");
            }
        }

        public void setState(ClientState state)
        {
            if(_state == state)
            {
                return;
            }
            _state = state;
            _chatView.Invoke("setState", state.ToString());
        }

        [ScriptableMember]
        public void logout()
        {
            _session.destroy();
            setState(ClientState.Disconnecting);
            _username = "";
        }

        //
        // This method invokes the send_async operation.
        //
        [ScriptableMember]
        public void send(string message)
        {
            if(_state != ClientState.Connected)
            {
                return;
            }
            if(message.Length > _maxMessageSize)
            {
                _chatView.Invoke("addMessage", 
                                 "<p>&lt;system-message&gt; - Message length exceeded, maximum length is " +
                                 _maxMessageSize + " characters.</p>");
            }
            else
            {
                AMI_ChatSession_sendI callback = new AMI_ChatSession_sendI(this, message);
                _chat.begin_send(message).whenCompleted(callback.sendMessageResponse, sendMessageException);
            }
        }

        //
        // Handler for send_async response.
        //
        public void sendMessageResponse(long timestamp, string message)
        {
            _chatView.Invoke("userSay", ChatUtils.formatTimestamp(timestamp), _username, message);
        }

        //
        // Handler for send_async exception.
        //
        private void sendMessageException(Ice.Exception ex)
        {
            if(ex is Chat.InvalidMessageException)
            {
                _chatView.Invoke("addMessage", 
                                 "<p>&lt;system-message&gt; - " + ((Chat.InvalidMessageException)ex).reason + "</p>");
            }
            else
            {
                _session.destroy();
                _chatView.Invoke("connectionLost", 
                                 "<p>&lt;system-message&gt;The connection with " +
                                 "the server was unexpectedly lost.<br/>" +
                                 "<br/><b>You can try to login again from the Login link in the top menu.</b></p>");
            }
        }

        public void initEvent(String[] users)
        {
            if(_state != ClientState.Connected && _state != ClientState.Connecting)
            {
                return;
            }
    
            for(int i = 0; i < users.Length; ++i)
            {
                _chatView.Invoke("addUser", users[i]);
            }
        }

        public void userJoinEvent(long timestamp, string name)
        {
            if(_state != ClientState.Connected && _state != ClientState.Connecting)
            {
                return;
            }
            _chatView.Invoke("addUser", name);
            _chatView.Invoke("addMessage", "<p>" + ChatUtils.formatTimestamp(timestamp) +
                             " - &lt;system-message&gt; - " + name + " joined.</p>");
        }

        public void userLeaveEvent(long timestamp, string name)
        {
            if(_state != ClientState.Connected && _state != ClientState.Connecting)
            {
                return;
            }
            _chatView.Invoke("delUser", name);
            _chatView.Invoke("addMessage", "<p>" + ChatUtils.formatTimestamp(timestamp) +
                             " - &lt;system-message&gt; - " + name + " left.</p>");
        }

        public void userSayEvent(long timestamp, string name, string message)
        {
            if(_state != ClientState.Connected && _state != ClientState.Connecting)
            {
                return;
            }
            _chatView.Invoke("userSay", ChatUtils.formatTimestamp(timestamp), name, message);
        }

        public string getUsername()
        {
            return _username;
        }

        private Glacier2.SessionHelper _session = null;
        private Chat.ChatSessionPrx _chat = null;
        private ClientState _state = ClientState.Disconnected;
        private string _defaultRouter = "";
        private string _policyProtocol = "";
        private string _username = "";
        private ScriptObject _chatView;
        private const int _maxMessageSize = 1024;
    }
}
