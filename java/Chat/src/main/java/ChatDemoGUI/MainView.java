// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package ChatDemoGUI;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JToolBar;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.ImageIcon;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;

import java.util.prefs.Preferences;
import java.util.prefs.BackingStoreException;

import com.jgoodies.looks.Options;
import com.jgoodies.looks.HeaderStyle;
import com.jgoodies.looks.BorderStyle;
import com.jgoodies.looks.plastic.PlasticLookAndFeel;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.builder.DefaultFormBuilder;
import com.jgoodies.forms.util.LayoutStyle;

public class MainView extends JFrame
{
    public static void main(final String[] args)
    {
        try
        {
            if(System.getProperty("os.name").startsWith("Mac OS")) // OS X L&F
            {
                System.setProperty("apple.laf.useScreenMenuBar", "true");
                System.setProperty("com.apple.mrj.application.apple.menu.about.name", "IceGrid Admin");
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            }
            else if(System.getProperty("os.name").startsWith("Windows"))
            {
                UIManager.setLookAndFeel("com.jgoodies.looks.windows.WindowsLookAndFeel");
            }
            else  // JGoodies L&F
            {
                UIManager.setLookAndFeel("com.jgoodies.looks.plastic.PlasticLookAndFeel");
            }
        }
        catch(Exception e)
        {
        }
        SwingUtilities.invokeLater(new Runnable()
        {
            public void run()
            {
                new MainView(args); // Create and set up the window.
            }
        });
    }

    private class MenuBar extends JMenuBar
    {
        private MenuBar()
        {
            putClientProperty(Options.HEADER_STYLE_KEY, HeaderStyle.BOTH);
            putClientProperty(PlasticLookAndFeel.BORDER_STYLE_KEY, BorderStyle.SEPARATOR);

            //
            // File menu
            //
            JMenu fileMenu = new JMenu("File");
            fileMenu.setMnemonic(java.awt.event.KeyEvent.VK_F);
            add(fileMenu);
            fileMenu.add(_login);
            fileMenu.add(_logout);
            if(!System.getProperty("os.name").startsWith("Mac OS"))
            {
                fileMenu.add(_exit);
            }

            //
            // Help menu
            //
            JMenu helpMenu = new JMenu("Help");
            helpMenu.setMnemonic(java.awt.event.KeyEvent.VK_H);
            add(helpMenu);
            helpMenu.add(_about);
        }
    }

    MainView(String[] args)
    {
        super("Chat Demo");
        setMinimumSize(new Dimension(minWidth, minHeight));
        _prefs = Preferences.userNodeForPackage(getClass());
        javax.swing.ImageIcon icon = getIcon("/icons/16x16/chat.png");
        if(icon != null)
        {
            setIconImage(icon.getImage());
        }
        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                storeWindowPrefs();
                _coordinator.exit();
            }
        });

        _about = new AbstractAction("About")
        {
            public void
            actionPerformed(ActionEvent e)
            {
                about();
            }
        };

        _login = new AbstractAction("Login")
        {
            public void actionPerformed(ActionEvent e)
            {
                _coordinator.setState(Coordinator.ClientState.Disconnected);
                _login.setEnabled(false);
            }
        };
        _login.setEnabled(false);

        _logout = new AbstractAction("Logout")
        {
            public void actionPerformed(ActionEvent e)
            {
                _logout.setEnabled(false);
                _chatView.storeWindowPrefs();
                _coordinator.logout();
            }
        };
        _logout.setEnabled(false);

        _exit = new AbstractAction("Exit")
        {
            public void actionPerformed(ActionEvent e)
            {
                _logout.setEnabled(false);
                _chatView.storeWindowPrefs();
                _coordinator.exit();
            }
        };
        _exit.putValue(Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke("alt F4"));

        DefaultListModel users = new DefaultListModel();
        _loginView = new LoginView(this, _prefs.node("Connection"));
        _chatView = new ChatView(_prefs, users);
        _statusBar = new StatusBarI();
        _coordinator = new Coordinator(this, _loginView, _chatView, users, args);
        _chatView.setCoordinator(_coordinator);
        _loginView.setCoordinator(_coordinator);

        FormLayout layout = new FormLayout("fill:pref:grow", "fill:pref:grow");
        DefaultFormBuilder builder = new DefaultFormBuilder(layout);
        builder.add(_loginView);
        builder.add(_chatView);

        setJMenuBar(new MenuBar());

        JPanel panel = new JPanel(new BorderLayout());
        panel.add(builder.getPanel(), BorderLayout.CENTER);
        panel.add(_statusBar, BorderLayout.PAGE_END);
        setContentPane(panel);

        _coordinator.setState(Coordinator.ClientState.Disconnected);

        setSize(minWidth, minHeight);
        if(!loadWindowPrefs())
        {
            ChatDemoGUIFactory.locateOnScreen(this);
        }
        setVisible(true);
    }

    public void setEnabled(boolean enabled)
    {
        _logout.setEnabled(enabled);
        _login.setEnabled(!enabled);
    }

    public void setConnected(boolean connected)
    {
        _statusBar.setConnected(connected);
    }

    private void about()
    {
        String text = "Chat Demo.\nCopyright \u00A9 2005-2016 ZeroC, Inc. All rights reserved.\n";
        JOptionPane.showMessageDialog(this, text, "About", JOptionPane.INFORMATION_MESSAGE);
    }

    private boolean loadWindowPrefs()
    {
        try
        {
            if(!_prefs.nodeExists("Window"))
            {
                return false;
            }
        }
        catch(BackingStoreException e)
        {
            return false;
        }

        Preferences windowPrefs = _prefs.node("Window");
        int x = windowPrefs.getInt("x", 0);
        int y = windowPrefs.getInt("y", 0);
        int width = windowPrefs.getInt("width", minWidth);
        int height = windowPrefs.getInt("height", minHeight);
        setBounds(new Rectangle(x, y, width, height));
        if(windowPrefs.getBoolean("maximized", false))
        {
            setExtendedState(Frame.MAXIMIZED_BOTH);
        }
        return true;
    }

    private void storeWindowPrefs()
    {
        Preferences windowPrefs = _prefs.node("Window");
        Rectangle rect = getBounds();
        windowPrefs.putInt("x", rect.x);
        windowPrefs.putInt("y", rect.y);
        windowPrefs.putInt("width", rect.width);
        windowPrefs.putInt("height", rect.height);
        windowPrefs.putBoolean("maximized", getExtendedState() == Frame.MAXIMIZED_BOTH);
    }

    static public ImageIcon getIcon(String path)
    {
        java.net.URL imgURL = MainView.class.getResource(path);
        if(imgURL == null)
        {
            return null;
        }
        else
        {
            return new ImageIcon(imgURL);
        }
    }

    private final Preferences _prefs;
    private final Coordinator _coordinator;
    private final LoginView _loginView;
    private final ChatView _chatView;
    private final StatusBarI _statusBar;
    private final Action _login;
    private final Action _logout;
    private final Action _exit;
    private final Action _about;

    // default size
    private static final int minWidth = 400;
    private static final int minHeight = 540;
}
