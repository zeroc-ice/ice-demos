// Copyright (c) ZeroC, Inc.

import Foundation

/// An in-memory implementation of PokeStore.
/// Note: This mock implementation is not thread-safe. A real implementation should support concurrent calls.
class InMemoryPokeStore: PokeStore {
    private var store: [String: [String]] = [:]

    func saveCollection(userId: String, pokemon: [String]) {
        if pokemon.isEmpty {
            store.removeValue(forKey: userId)
        } else {
            store[userId] = pokemon
        }
    }

    func retrieveCollection(userId: String) -> [String] {
        return store[userId] ?? []
    }
}
