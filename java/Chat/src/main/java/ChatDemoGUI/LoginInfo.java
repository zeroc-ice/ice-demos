// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
//
// **********************************************************************

package ChatDemoGUI;

import java.util.prefs.Preferences;
import java.util.prefs.BackingStoreException;

class LoginInfo
{
    public LoginInfo()
    {
    }

    public LoginInfo(Preferences connectionPrefs)
    {
        _connectionPrefs = connectionPrefs;
        load();
    }

    public void load()
    {
        username = _connectionPrefs.get("chatdemo.username", username);
    }

    public void save()
    {
        _connectionPrefs.put("chatdemo.username", username);
    }

    public String username = System.getProperty("user.name");
    public String password = "";
    private Preferences _connectionPrefs;
}
