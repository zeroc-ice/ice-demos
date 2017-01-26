// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package ChatDemoGUI;

import java.awt.BorderLayout;

import javax.swing.border.EmptyBorder;
import javax.swing.SwingConstants;
import javax.swing.JPanel;
import javax.swing.JLabel;

public class StatusBarI extends JPanel
{
    StatusBarI()
    {
        super(new BorderLayout());
        setBorder(new EmptyBorder(7, 7, 7, 7));
        _connectedLabel.setHorizontalAlignment(SwingConstants.RIGHT);
        add(_connectedLabel, BorderLayout.LINE_START);
    }

    public void setConnected(boolean connected)
    {
        if(connected)
        {
            _connectedLabel.setText("Online");
        }
        else
        {
            _connectedLabel.setText("Offline");
        }
    }

    private final JLabel _connectedLabel = new JLabel("Offline");
}
