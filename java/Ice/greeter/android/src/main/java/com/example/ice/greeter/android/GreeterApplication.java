// Copyright (c) ZeroC, Inc.

package com.example.ice.greeter.android;

import android.app.Application;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Util;

public class GreeterApplication extends Application {
    private Communicator communicator;

    @Override
    public void onCreate() {
        super.onCreate();
        // Initialize Ice communicator when the application starts
        try {
            communicator = Util.initialize();
        } catch (Exception e) {
            // Handle initialization error
            e.printStackTrace();
        }
    }

    @Override
    public void onTerminate() {
        // Destroy Ice communicator when the application terminates
        if (communicator != null) {
            try {
                communicator.destroy();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        super.onTerminate();
    }

    public Communicator getCommunicator() {
        return communicator;
    }
}