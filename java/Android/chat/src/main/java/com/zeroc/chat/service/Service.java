// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.chat.service;

public interface Service
{
    /**
     *
     * Set a new session listener.
     *
     * @param listener
     *            The new session listener.
     *
     * @return true if a login is already in progress, false otherwise.
     *
     */
    boolean setSessionListener(SessionListener listener);

    /**
     *
     * Establish a session with the chat server.
     *
     * @param username
     *            The username.
     *
     * @param password
     *            The password.
     *
     */
    void login(String username, String password);

    /**
     *
     * Get the last login failure error message.
     *
     * @return The last login error.
     *
     */
    String getLoginError();

    /**
     *
     * Called to logout from the current session.
     *
     */
    void logout();

    /**
     *
     * Add a chat room listener.
     *
     * @param listener
     *            The listener.
     *
     * @param replay
     *            True if all received events should be replayed.
     *
     * @return The name of the host of the session.
     *
     * @throws NoSessionException
     *             if there is no established session.
     *
     */
    String addChatRoomListener(ChatRoomListener listener, boolean replay) throws NoSessionException;

    /**
     *
     * Remove a chat room listener.
     *
     * @param listener
     *            The listener.
     *
     */
    void removeChatRoomListener(ChatRoomListener listener);

    /**
     *
     * Send a message to the chat room.
     *
     */
    void send(String message);

    /**
     *
     * Get the last session error message.
     *
     * @return The last session error.
     *
     */
    String getSessionError();
}
