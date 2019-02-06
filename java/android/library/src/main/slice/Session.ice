//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

#include <Library.ice>

["java:package:com.zeroc.demos.android.library"]
module Demo
{

/**
 *
 * The session object. This is used to retrieve a per-session library
 * on behalf of the client. If the session is not refreshed on a
 * periodic basis, it will be automatically destroyed.
 *
 */
interface Session
{
    /**
     *
     * Get the library object.
     *
     * @return A proxy for the new library.
     *
     **/
    Library* getLibrary();

    /**
     *
     * Destroy the session.
     *
     **/
    void destroy();
}

/**
 *
 * Interface to create new sessions.
 *
 **/
interface SessionFactory
{
    /**
     *
     * Create a session.
     *
     * @return A proxy to the session.
     *
     **/
    Session* create();
}

}
