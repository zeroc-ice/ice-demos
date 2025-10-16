// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

import java.util.List;

interface PokeStore {
    /**
     * Saves the Pokémon collection of a specific user.
     * @param userId the user's ID
     * @param pokemon the Pokémon collection to save
     */
    void saveCollection(String userId, List<String> pokemon);

    /**
     * Retrieves the Pokémon collection of a specific user.
     * @param userId the user's ID
     * @return the saved Pokémon collection, or {@code null} if no collection was saved for {@code userId}.
     *     The returned list is immutable. Attempting to modify it will throw an UnsupportedOperationException.
     */
    List<String> retrieveCollection(String userId);
}
