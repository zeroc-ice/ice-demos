// Copyright (c) ZeroC, Inc.

#ifndef POKE_STORE_H
#define POKE_STORE_H

#include "PokeBox.h"

namespace Server
{
    /// Represents a Pokémon storage system.
    class PokeStore
    {
    public:
        virtual ~PokeStore() = default;

        /// Saves the Pokémon collection for a specific user.
        /// @param userId The user ID.
        /// @param pokemon The Pokémon collection to save.
        virtual void saveCollection(const std::string& userId, CatchThemAll::PokemonList pokemon) = 0;

        /// Retrieves the Pokémon collection for a specific user.
        /// @param userId The user ID.
        /// @returns The saved Pokémon collection, or an empty list if no collection was saved for @p userId.
        [[nodiscard]] virtual CatchThemAll::PokemonList retrieveCollection(const std::string& userId) const = 0;
    };

    using PokeStorePtr = std::shared_ptr<PokeStore>;
}

#endif
