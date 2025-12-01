// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

/// Represents a Pokémon storage system.
protocol PokeStore {
    /// Saves the Pokémon collection for a specific user.
    /// - Parameters:
    ///   - userId: The user ID.
    ///   - pokemon: The Pokémon collection to save.
    func saveCollection(userId: String, pokemon: [String])

    /// Retrieves the Pokémon collection for a specific user.
    /// - Parameter userId: The user ID.
    /// - Returns: The saved Pokémon collection, or an empty list if no collection was saved.
    func retrieveCollection(userId: String) -> [String]
}
