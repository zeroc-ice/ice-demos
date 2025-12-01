// Copyright (c) ZeroC, Inc.

/// An in-memory implementation of PokeStore.
actor InMemoryPokeStore: PokeStore {
    private var store: [String: [String]] = [:]

    func saveCollection(userId: String, pokemon: [String]) {
        if pokemon.isEmpty {
            store.removeValue(forKey: userId)
        } else {
            store[userId] = pokemon
        }
    }

    func retrieveCollection(userId: String) -> [String] {
        store[userId] ?? []
    }
}
