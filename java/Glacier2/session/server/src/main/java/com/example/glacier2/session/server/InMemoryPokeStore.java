// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.server;

import java.util.ArrayList;
import java.util.Hap;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

class InMemoryPokeStore implements PokeStore {
    private final Map<String, List<String>> _store = new HashMap<>();

    public void saveCollection(String userId, List<String> pokemon) {
        if (pokemon.isEmpty()) {
            _store.remove(userId);
        } else {
            _store.put(userId, List.copyOf(pokemon)); // 'copyOf' returns an immutable list.
        }
    }

    public List<String> retrieveCollection(String userId) {
        return _store.getOrDefault(userId, null);
    }
}
