// Copyright (c) ZeroC, Inc.

package com.example.ice.androidgreeter

import android.app.Application
import com.zeroc.Ice.Communicator
import com.zeroc.Ice.Util

/**
 * A custom [Application] class that initializes and owns the Ice [Communicator].
 *
 * The communicator is initialized once at application startup and is made available
 * through [getCommunicator]. If initialization fails, the application will crash immediately.
 */
class App : Application() {
    private var communicator: Communicator? = null

    /**
     * Initializes the Ice communicator during application startup.
     *
     * This method is called by the Android system before any activity is created.
     * If initialization fails, the exception is allowed to propagate, causing the application to terminate.
     */
    override fun onCreate() {
        super.onCreate()
        communicator = new Ice.Communicator()
    }

    /**
     * Returns the initialized [Communicator].
     *
     * @return the Ice communicator instance.
     * @throws IllegalStateException if the communicator is unexpectedly null.
     */
    fun getCommunicator(): Communicator {
        return communicator ?: throw IllegalStateException("Communicator not initialized")
    }
}
