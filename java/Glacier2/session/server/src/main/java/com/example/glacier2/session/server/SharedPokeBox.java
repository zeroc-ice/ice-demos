// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

import com.example.catchthemall.PokeBox;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.DispatchException;
import com.zeroc.Ice.ReplyStatus;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * {@code SharedPokeBox} is an Ice servant that implements Slice interface 'PokeBox'. The same shared servant
 * implements all PokeBox objects; this is doable because all the state is stored in the {@link PokeStore}.
 */
class SharedPokeBox implements PokeBox {
    private final PokeStore _pokeStore;
    private final UserIdResolver _userIdResolver;

    /**
     * Constructs a {@code SharedPokeBox} servant.
     *
     * @param pokeStore the Poke-store
     * @param userIdResolver the user ID resolver
     */
    SharedPokeBox(PokeStore pokeStore, UserIdResolver userIdResolver) {
        _pokeStore = pokeStore;
        _userIdResolver = userIdResolver;
    }

    // Retrieve the Pokémon collection for the user associated with the current session.
    @Override
    public List<String> getInventory(Current current) {
        return _pokeStore.retrieveCollection(getUserId(current));
    }

    // Add new Pokémon to the Pokémon collection for the user associated with the current session.
    @Override
    public void caught(List<String> pokemon, Current current) {
        String userId = getUserId(current);

        List<String> newPokemon = new ArrayList<>(pokemon);
        List<String> savedPokemon = _pokeStore.retrieveCollection(userId);
        if (savedPokemon != null) {
            newPokemon.addAll(savedPokemon);
        }

        // Sort the collection to make it easier to see the Pokémon that have been caught.
        Collections.sort(newPokemon);
        _pokeStore.saveCollection(userId, newPokemon);
    }

    @Override
    public void releaseAll(Current current) {
        _pokeStore.saveCollection(getUserId(current), Collections.emptyList());
    }

    /**
     * Retrieves the user ID associated with the current session.
     *
     * @param current information about the incoming request being dispatched
     * @return the user ID associated with the current session.
     */
    private String getUserId(Current current) {
        String userId = _userIdResolver.getUserId(current.id.name);
        if (userId == null) {
            throw new DispatchException(ReplyStatus.Unauthorized.value(), "Invalid session token");
        }
        return userId;
    }
}
