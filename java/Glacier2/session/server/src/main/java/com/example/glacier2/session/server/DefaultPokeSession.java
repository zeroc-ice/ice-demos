// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

import com.example.catchthemall.PokeBoxPrx;
import com.example.catchthemall.PokeSession;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.Identity;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Glacier2.SessionControlPrx;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;

class DefaultPokeSession implements AsyncPokeSession {
    /** The object adapter that hosts this servant and the PokeBox objects. */
    private final ObjectAdapter _adapter;

    /**
     * A proxy to the SessionControl object hosted by the Glacier2 router.
     * This proxy allows us to control the Glacier2 session, in particular to destroy it.
     * In this demo, that's the only per-session state maintained by {@code DefaultPokeSession}.
     */
    private final SessionControlPrx _sessionControl;

    private final UserIdResolver _userIdResolver;

    /**
     * Constructs a {@code DefaultPokeSession} servant.
     * @param adapter the object adapter that hosts this servant and the PokeBox objects
     * @param sessionControl the session control proxy
     * @param userIdResolver the user ID resolver
     */
    DefaultPokeSession(ObjectAdapter adapter, SessionControlPrx sessionControl, UserIdResolver userIdResolver) {
        _adapter = adapter;
        _sessionControl = sessionControl;
        _userIdResolver = userIdResolver;
    }

    @Override
    public CompletionStage<PokeBoxPrx> getPokeBoxAsync(Current current) {
        // The session token is the name component of the session identity;
        // we use it for the identity of the PokeBox object as well.
        ObjectPrx proxy = _adapter.createProxy(new Identity(current.id.name, "PokeBox"));
        return CompletableFuture.completedFuture(PokeBoxPrx.uncheckedCast(proxy));
    }

    @Override
    public CompletionStage<Void> destroyAsync(Current current) {
        System.out.println("Destroying session #" + current.id.name);

        // Remove the token from the user ID resolver, since the token is no longer valid.
        _userIdResolver.removeToken(current.id.name);

        // Remove this servant from the object adapter.
        // A new call to this session object will fail with ObjectNotExistException.
        _adapter.remove(current.id);

        // Destroy the session in the Glacier2 router.
        return _sessionControl.destroyAsync();
    }
}
