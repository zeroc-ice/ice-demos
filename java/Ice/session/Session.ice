//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.session"]
module Demo
{
    interface Hello
    {
        idempotent void sayHello();
    }

    //
    // The session object. This is used to create per-session objects on
    // behalf of the client.
    //
    interface Session
    {
        //
        // Create a new per-session hello object. The created object will
        // be automatically destroyed when the session is destroyed.
        //
        Hello* createHello();

        idempotent string getName();

        //
        // Destroy the session explicitly.
        //
        void destroy();
    }

    interface SessionFactory
    {
        //
        // Create a session with the given name. Note that the name is
        // only used for diagnostic purposes. It is not used as unique
        // session id.
        //
        Session* create(string name);

        void shutdown();
    }
}
