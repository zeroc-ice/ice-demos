// Copyright (c) ZeroC, Inc.

import Foundation
import Ice

/// SharedPokeBox is an Ice servant that implements Slice interface PokeBox. The same shared servant
/// implements all PokeBox objects; this is doable because all the state is stored in the PokeStore.
class SharedPokeBox: CatchThemAll.PokeBox {
    private let pokeStore: PokeStore
    private let userIdResolver: UserIdResolver

    /// Constructs a SharedPokeBox servant.
    /// - Parameters:
    ///   - pokeStore: The Pokémon store.
    ///   - userIdResolver: The user ID resolver.
    init(pokeStore: PokeStore, userIdResolver: UserIdResolver) {
        self.pokeStore = pokeStore
        self.userIdResolver = userIdResolver
    }

    func getInventory(current: Ice.Current) async throws -> [String] {
        return pokeStore.retrieveCollection(userId: try getUserId(current: current))
    }

    func caught(pokemon: [String], current: Ice.Current) async throws {
        let userId = try getUserId(current: current)
        var savedPokemon = pokeStore.retrieveCollection(userId: userId)

        savedPokemon.append(contentsOf: pokemon)

        // Sort the collection to make it easier to see the Pokémon that have been caught.
        savedPokemon.sort()
        pokeStore.saveCollection(userId: userId, pokemon: savedPokemon)
    }

    func releaseAll(current: Ice.Current) async throws {
        pokeStore.saveCollection(userId: try getUserId(current: current), pokemon: [])
    }

    /// Retrieves the user ID associated with the current session.
    /// - Parameter current: Information about the incoming request being dispatched.
    /// - Returns: The user ID associated with the current session.
    /// - Throws: DispatchException if the session token is invalid.
    private func getUserId(current: Ice.Current) throws -> String {
        guard let userId = userIdResolver.getUserId(current.id.name) else {
            throw Ice.DispatchException(replyStatus: .unauthorized)
        }
        return userId
    }
}
