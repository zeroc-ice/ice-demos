// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

import com.zeroc.Ice.Current;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Glacier2.SessionControlPrx;
import com.zeroc.Glacier2.SessionPrx;

import java.util.HashMap;
import java.util.Map;

/**
 * {@code SessionManager} is an Ice servant that implements Slice interface 'Glacier2::SessionManager'.
 * It creates sessions and resolves user IDs from session tokens.
 *
 * <p>This demo implementation is not thread-safe. A real implementation should support concurrent calls.
 */
class SessionManager implements com.zeroc.Glacier2.SessionManager, UserIdResolver {
    private final ObjectAdapter _sessionAdapter;

    private final Map<String, String> _tokenToUserId = new HashMap<>();

    /**
     * Constructs a SessionManager servant.
     *
     * @param adapter the object adapter that hosts the session servants
     */
    SessionManager(ObjectAdapter adapter) {
        _sessionAdapter = adapter;
    }

    @Override
    public SessionPrx create(String userId, SessionControlPrx sessionControl, Current current) {
        // sessionControl is not null because we configured Glacier2.Server.Endpoints
        // in the Glacier2 router configuration file.
        assert sessionControl != null;

        // Create a new session servant and add it to the adapter with a UUID identity.
        // The name component of the identity is the session token.
        ObjectPrx proxy = _sessionAdapter.addWithUUID(new DefaultPokeSession(_sessionAdapter, sessionControl, this));

        String sessionToken = proxy.ice_getIdentity().name;
        _tokenToUserId.put(sessionToken, userId);

        System.out.println("Created session #" + sessionToken + " for user '" + userId + "'");
        return SessionPrx.uncheckedCast(proxy);
    }

    @Override
    public String getUserId(String token) {
        return _tokenToUserId.get(token);
    }

    @Override
    public void removeToken(String token) {
        _tokenToUserId.remove(token);
    }
}
