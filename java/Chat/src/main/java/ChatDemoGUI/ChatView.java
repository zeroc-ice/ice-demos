// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package ChatDemoGUI;

import java.awt.Component;
import java.awt.event.ComponentListener;
import java.awt.event.ComponentEvent;

import java.awt.BorderLayout;
import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;
import java.awt.Dimension;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JPanel;
import javax.swing.JList;
import javax.swing.JPopupMenu;
import javax.swing.DefaultListCellRenderer;
import javax.swing.JLabel;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JScrollBar;
import javax.swing.DefaultListModel;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultEditorKit;

import java.util.prefs.Preferences;
import java.util.prefs.BackingStoreException;

public class ChatView extends JPanel
{
    @SuppressWarnings("unchecked")
    ChatView(Preferences prefs, DefaultListModel users)
    {
        _prefs = prefs;
        setLayout(new BorderLayout());

        // Build the user list.
        _userList = new JList();
        _userList.setCellRenderer(new UserListCellRender());
        _userList.setModel(users);

        // Build the JTextArea that shows the chat conversation.
        _conversationText = new JTextArea("");
        _conversationText.setLineWrap(true);
        _conversationText.setEditable(false);

        final JPopupMenu textMenu = new JPopupMenu();
        textMenu.add(new DefaultEditorKit.CopyAction());
        textMenu.pack();

        _conversationText.addMouseListener(new MouseAdapter()
            {
                public void mousePressed(MouseEvent e)
                {
                    if (e.isPopupTrigger()) // BUTTON3 is the right mouse button
                    {
                        textMenu.show(_conversationText, e.getX(), e.getY());
                    }
                }
            });

        // Build the JTextArea where the user writes input messages.
        _conversationInput = new JTextArea("");
        _conversationInput.setLineWrap(true);
        _conversationInput.setEditable(true);
        _conversationInput.addKeyListener(new InputLineKeyListener());

        // Build the split panes.
        _horizontalSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);

        JScrollPane usersScroll = ChatDemoGUIFactory.createStrippedScrollPane(_userList);
        usersScroll.setMinimumSize(new Dimension(100, 100));
        _horizontalSplit.setLeftComponent(usersScroll);

        _conversationTextScroll = ChatDemoGUIFactory.createStrippedScrollPane(_conversationText);
        _conversationTextScroll.setMinimumSize(new Dimension(100, 100));
        _conversationTextScroll.setPreferredSize(new Dimension(100, 100));
        _horizontalSplit.setRightComponent(_conversationTextScroll);

        _verticalSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        _verticalSplit.setTopComponent(_horizontalSplit);

        JScrollPane conversationInputScroll = ChatDemoGUIFactory.createStrippedScrollPane(_conversationInput);
        conversationInputScroll.setMinimumSize(new Dimension(100, 100));
        conversationInputScroll.setPreferredSize(new Dimension(100, 100));
        _verticalSplit.setBottomComponent(conversationInputScroll);

        _horizontalSplit.setResizeWeight(0.1);
        _verticalSplit.setResizeWeight(0.9);

        _conversationText.addComponentListener(new ComponentListener()
            {
                public void componentResized(ComponentEvent e)
                {
                    JScrollBar vertivalScrollbar = _conversationTextScroll.getVerticalScrollBar();
                    vertivalScrollbar.setValue(vertivalScrollbar.getMaximum());
                }

                public void  componentHidden(ComponentEvent e)
                {
                }

                public void  componentMoved(ComponentEvent e)
                {
                }

                public void  componentShown(ComponentEvent e)
                {
                }
            });

        add(_verticalSplit, BorderLayout.CENTER);
    }

    public void setCoordinator(Coordinator coordinator)
    {
        _coordinator = coordinator;
    }

    public void appendMessage(String message)
    {
        Document doc = (Document) _conversationText.getDocument();
        Element e = doc.getDefaultRootElement();
        AttributeSet attr = e.getAttributes().copyAttributes();
        try
        {
            doc.insertString(doc.getLength(), message + "\n", attr);
        }
        catch(BadLocationException ex)
        {
        }
        _conversationText.setCaretPosition(doc.getLength());
    }

    public void appendError(String error)
    {
        appendMessage(error);
        _conversationInput.setEditable(false);
    }

    class UserListCellRender extends DefaultListCellRenderer
    {
        public Component
        getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean hasFocus)
        {
            JLabel label = (JLabel)super.getListCellRendererComponent(list, value, index, isSelected, hasFocus);
            label.setIcon(MainView.getIcon("/icons/16x16/user.png"));
            return label;
        }
    }

    //
    // Event handler for input messages.
    //
    class InputLineKeyListener implements KeyListener
    {
        public void keyTyped(KeyEvent e)
        {
            if(e.getKeyChar() == KeyEvent.VK_ENTER)
            {
                try
                {
                    Document doc = _conversationInput.getDocument();
                    String msg = doc.getText(0, doc.getLength()).trim();
                    if(msg.length() > 0)
                    {
                        _coordinator.sendMessage(msg);
                    }
                }
                catch(BadLocationException ex)
                {
                }
                _conversationInput.setText("");
            }
        }

        public void keyPressed(KeyEvent e)
        {
        }

        public void keyReleased(KeyEvent e)
        {
        }
    }

    public void storeWindowPrefs()
    {
        Preferences windowPrefs = _prefs.node("ChatView");
        windowPrefs.putInt("hSplitLocation", _horizontalSplit.getDividerLocation());
        windowPrefs.putInt("vSplitLocation", _verticalSplit.getDividerLocation());
    }

    public boolean loadWindowPrefs()
    {
        try
        {
            if(!_prefs.nodeExists("ChatView"))
            {
                return false;
            }
        }
        catch(BackingStoreException ex)
        {
            return false;
        }

        Preferences windowPrefs = _prefs.node("ChatView");
        _horizontalSplit.setDividerLocation(windowPrefs.getInt("hSplitLocation", 120));
        _verticalSplit.setDividerLocation(windowPrefs.getInt("vSplitLocation", 50));
        return true;
    }

    public void setConnected(boolean connected)
    {
        setVisible(connected);
        if(connected)
        {
            loadWindowPrefs();
            _conversationInput.setEditable(true);
            if(!_conversationInput.hasFocus())
            {
                _conversationInput.requestFocus();
            }
        }
        else
        {
            storeWindowPrefs();
            _conversationText.setText("");
            _conversationInput.setText("");
        }
    }

    private final JList _userList;
    private final JScrollPane _conversationTextScroll;
    private final JTextArea _conversationText;
    private final JTextArea _conversationInput;
    private final JSplitPane _verticalSplit;
    private final JSplitPane _horizontalSplit;
    private Coordinator _coordinator;
    private final Preferences _prefs;
}
