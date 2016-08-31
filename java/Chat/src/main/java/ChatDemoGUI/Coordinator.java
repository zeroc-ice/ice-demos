// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package ChatDemoGUI;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.Cursor;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Frame;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.DefaultListModel;
import javax.swing.SwingUtilities;
import javax.swing.JOptionPane;

import javax.swing.JDialog;
import javax.swing.JPopupMenu;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import javax.swing.text.DefaultEditorKit;
import javax.swing.ImageIcon;

import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.builder.DefaultFormBuilder;
import com.jgoodies.forms.factories.Borders;

import com.zeroc.Glacier2.SessionFactoryHelper;
import com.zeroc.Glacier2.SessionHelper;
import com.zeroc.Glacier2.SessionCallback;
import com.zeroc.Glacier2.SessionNotExistException;

class Coordinator
{
    public enum ClientState { Disconnected, Connecting, Connected, Disconnecting }

    public Coordinator(MainView mainView, LoginView loginView, ChatView chatView, DefaultListModel users,
                       String[] args)
    {
        _args = args;
        _mainView = mainView;
        _loginView = loginView;
        _chatView = chatView;
        _users = users;

        com.zeroc.Ice.InitializationData initData = new com.zeroc.Ice.InitializationData();

        com.zeroc.Ice.Util.CreatePropertiesResult cpr = com.zeroc.Ice.Util.createProperties(_args);
        initData.properties = cpr.properties;
        _args = cpr.args;

        initData.properties.setProperty("Ice.Plugin.IceSSL", "com.zeroc.IceSSL.PluginFactory");

        //
        // Set Ice.Default.Router if not set.
        //
        if(initData.properties.getProperty("Ice.Default.Router").isEmpty())
        {
            initData.properties.setProperty("IceSSL.UsePlatformCAs", "1");
            initData.properties.setProperty("IceSSL.CheckCertName", "1");
            initData.properties.setProperty("Ice.Default.Router",
                                            "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2");
        }

        initData.dispatcher = (runnable, connection) ->
            {
                if(_exit)  // The GUI is being destroyed, don't use the GUI thread any more
                {
                    runnable.run();
                }
                else
                {
                    SwingUtilities.invokeLater(runnable);
                }
            };

        final Coordinator coordinator = this;
        _factory = new SessionFactoryHelper(initData, new SessionCallback()
            {
                public void connected(final SessionHelper session)
                    throws SessionNotExistException
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

                    Chat.ChatRoomCallbackPrx callback = Chat.ChatRoomCallbackPrx.uncheckedCast(
                        _session.addWithUUID(new ChatRoomCallbackI(coordinator)));

                    _chat = Chat.ChatSessionPrx.uncheckedCast(_session.session());
                    try
                    {
                        _chat.setCallbackAsync(callback).whenComplete((result, ex) ->
                            {
                                if(ex == null)
                                {
                                    _info.save();
                                    _username = ChatUtils.formatUsername(_info.username);
                                    setState(ClientState.Connected);
                                }
                                else
                                {
                                    destroySession();
                                }
                            });
                    }
                    catch(com.zeroc.Ice.CommunicatorDestroyedException ex)
                    {
                        // Ignore client session was destroyed.
                    }
                }

                public void disconnected(SessionHelper session)
                {
                    //
                    // Ignore callbacks during shutdown.
                    //
                    if(_exit)
                    {
                        return;
                    }

                    _username = "";
                    if(_state == ClientState.Disconnecting) // Connection closed by user logout/exit
                    {
                        setState(ClientState.Disconnected);
                    }
                    else if(_state == ClientState.Connected) // Connection lost while user was chatting
                    {
                        setError("<system-message> - The connection with the server was unexpectedly lost.\n" +
                                 "You can try to login again from the File menu.");
                    }
                    else // Connection lost while user was connecting
                    {
                        setError("<system-message> - The connection with the server was unexpectedly lost.\n" +
                                 "Try again.");
                    }
                }

                public void connectFailed(SessionHelper session, Throwable exception)
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
                    catch(final com.zeroc.Glacier2.CannotCreateSessionException ex)
                    {
                        setError("Login failed (Glacier2.CannotCreateSessionException):\n" + ex.reason);
                    }
                    catch(final com.zeroc.Glacier2.PermissionDeniedException ex)
                    {
                        setError("Login failed (Glacier2.PermissionDeniedException):\n" + ex.reason);
                    }
                    catch(com.zeroc.Ice.Exception ex)
                    {
                        setError("Login failed (" + ex.ice_name() + ").\n" + "Please check your configuration.");
                    }
                    catch(final Throwable ex)
                    {
                        setError("Login failed:\n" + ChatUtils.stack2string(ex));
                    }
                }

                public void createdCommunicator(SessionHelper session)
                {
                }
            });
    }

    public void login(LoginInfo info)
    {
        setState(ClientState.Connecting);
        _info = info;
        _session = _factory.connect(info.username, info.password);
    }

    public void logout()
    {
        setState(ClientState.Disconnecting);
        destroySession();
    }

    public void exit()
    {
        _exit = true;
        com.zeroc.Ice.Communicator communicator = _session == null ? null : _session.communicator();
        destroySession();
        _mainView.dispose();
        if(communicator != null)
        {
            communicator.waitForShutdown();
        }
        System.exit(0);
    }

    @SuppressWarnings("unchecked")
    public void initEvent(final String[] users)
    {
        for(int cont = 0; cont < users.length; ++cont)
        {
            _users.addElement(users[cont]);
        }
    }

    @SuppressWarnings("unchecked")
    public void userJoinEvent(final long timestamp, final String name)
    {
        _users.addElement(name);
        _chatView.appendMessage(ChatUtils.formatTimestamp(timestamp) + " - <system-message> - " + name + " joined.");
    }

    public void userLeaveEvent(final long timestamp, final String name)
    {
        int index = _users.indexOf(name);
        if(index != -1)
        {
            _users.remove(index);
            _chatView.appendMessage(ChatUtils.formatTimestamp(timestamp) + " - " + "<system-message> - " +
                                    name + " left.");
        }
    }

    public void userSayEvent(final long timestamp, final String name, final String message)
    {
        _chatView.appendMessage(ChatUtils.formatTimestamp(timestamp) + " - <" + name + "> " +
                                ChatUtils.unstripHtml(message));
    }

    public void sendMessage(final String message)
    {

        if(_chat != null)
        {
            if(message.length() > _maxMessageSize)
            {
                _chatView.appendMessage("<system-message> - Message length exceeded, maximum length is " +
                                        _maxMessageSize + " characters.");
            }
            else
            {
                try
                {
                    _chat.sendAsync(message).whenComplete((timestamp, ex) ->
                        {
                            if(ex == null)
                            {
                                userSayEvent(timestamp, _username, message);
                            }
                            else if(ex instanceof Chat.InvalidMessageException)
                            {
                                Chat.InvalidMessageException e = (Chat.InvalidMessageException)ex;
                                _chatView.appendMessage("<system-message> - " + e.reason);
                            }
                            else
                            {
                                destroySession();
                            }
                        });
                }
                catch(com.zeroc.Ice.CommunicatorDestroyedException ex)
                {
                    // Ignore client session was destroyed.
                }
            }
        }
    }

    public void setState(ClientState state)
    {
        _state = state;
        if(state == ClientState.Disconnected)
        {
            _loginView.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
            _chatView.setConnected(false);
            _loginView.setEnabled(true);
            _loginView.setConnected(false);
            _mainView.setEnabled(false);
            _mainView.setConnected(false);
            _users.clear();
        }
        else if(state == ClientState.Connecting)
        {
            _loginView.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
            _loginView.setEnabled(false);
            _mainView.setEnabled(false);
        }
        else if(state == ClientState.Connected)
        {
            _chatView.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
            _loginView.setConnected(true);
            _chatView.setConnected(true);
            _mainView.setEnabled(true);
            _mainView.setConnected(true);
        }
        else if(state == ClientState.Disconnecting)
        {
            _chatView.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
            _mainView.setEnabled(false);
        }
    }

    public void setError(final String message)
    {
        //
        // Don't display errors at that point GUI is being destroyed.
        //
        if(_exit == true)
        {
            return;
        }
        if(_state != ClientState.Connected)
        {
            setState(ClientState.Disconnected);
            ErrorView errorView = new ErrorView(_mainView, message);
            errorView.setSize(430, 200);
            errorView.setMinimumSize(new Dimension(430, 200));
            errorView.setMaximumSize(new Dimension(430, 200));
            errorView.setResizable(false);
            ChatDemoGUIFactory.centerComponent(errorView, _mainView);
            errorView.setVisible(true);
        }
        else
        {
            _mainView.setEnabled(false);
            _mainView.setConnected(false);
            _chatView.appendError(message);
        }
    }

    public String getUsername()
    {
        return _username;
    }

    public class ErrorView extends JDialog
    {
        private JButton bttClose = new JButton("Close");
        ErrorView(Frame frame, String message)
        {
            super(frame, true);
            setTitle("Error - Chat Demo");
            ActionListener closeListener = new ActionListener()
            {
                public void actionPerformed(ActionEvent e)
                {
                    setVisible(false);
                }
            };
            bttClose.addActionListener(closeListener);

            final JTextArea txtErrorMessage = new JTextArea(message, 5, 30);
            txtErrorMessage.setLineWrap(true);
            JPanel errorPanel = null; // Build the error panel.
            {
                FormLayout layout = new FormLayout("center:pref:grow", "center:pref:grow");
                DefaultFormBuilder builder = new DefaultFormBuilder(layout);
                builder.border(Borders.DIALOG);
                javax.swing.ImageIcon icon = MainView.getIcon("/icons/32x32/error.png");
                if(icon != null)
                {
                    builder.append(new JLabel(icon));
                    builder.nextLine();
                }
                txtErrorMessage.setEditable(false);
                builder.append(ChatDemoGUIFactory.createStrippedScrollPane(txtErrorMessage));
                errorPanel = builder.getPanel();
            }

            JPanel actionsPanel = null; // Build a panel for put actions.
            {
                FormLayout layout = new FormLayout("center:3dlu:grow", "pref");
                DefaultFormBuilder builder = new DefaultFormBuilder(layout);
                builder.append(bttClose);
                actionsPanel = builder.getPanel();
                getRootPane().setDefaultButton(bttClose);
            }

            FormLayout layout = new FormLayout("fill:pref:grow", "pref");
            DefaultFormBuilder builder = new DefaultFormBuilder(layout);
            builder.append(errorPanel);
            builder.nextLine();
            builder.append(actionsPanel);

            setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
            setContentPane(builder.getPanel());

            final JPopupMenu textMenu = new JPopupMenu();
            textMenu.add(new DefaultEditorKit.CopyAction());
            textMenu.pack();

            txtErrorMessage.addMouseListener(new MouseAdapter()
                {
                    public void mousePressed(MouseEvent e)
                    {
                        if (e.isPopupTrigger()) // BUTTON3 is the right mouse button
                        {
                            textMenu.show(txtErrorMessage, e.getX(), e.getY());
                        }
                    }
                });
        }
    }

    protected void
    destroySession()
    {
        final com.zeroc.Glacier2.SessionHelper s = _session;
        _session = null;
        _chat = null;

        if(s != null)
        {
            s.destroy();
        }
    }

    private final SessionFactoryHelper _factory;
    private ClientState _state;
    private String[] _args;
    private final MainView _mainView;
    private final ChatView _chatView;
    private final LoginView _loginView;
    private final DefaultListModel _users;
    private LoginInfo _info = new LoginInfo();
    private SessionHelper _session = null;
    private Object _sessionMonitor = new java.lang.Object();
    private Chat.ChatSessionPrx _chat = null;
    private String _username = "";
    private static final int _maxMessageSize = 1024;
    private boolean _exit = false;
}
