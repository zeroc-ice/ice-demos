// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
//
// **********************************************************************

package ChatDemoGUI;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;

import javax.swing.JScrollPane;

//
// Helper methods to create the UI.
//
final public class ChatDemoGUIFactory
{
    public static void locateOnScreen(Component component)
    {
        Dimension paneSize = component.getSize();
        Dimension screenSize = component.getToolkit().getScreenSize();
        component.setLocation((screenSize.width - paneSize.width) / 2, (screenSize.height - paneSize.height) / 2);
    }

    public static void centerComponent(Component component, Component base)
    {
        Dimension baseSize = base.getSize();
        Point basePoint = base.getLocation();
        Dimension componentSize = component.getSize();
        component.setLocation(basePoint.x + (baseSize.width - componentSize.width) / 2,
                              basePoint.y + (baseSize.height - componentSize.height) / 2);
    }

    public static JScrollPane createStrippedScrollPane(Component component)
    {
        JScrollPane scrollPane = new JScrollPane(component);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.setBorder(null);
        return scrollPane;
    }
}
