// Copyright (c) ZeroC, Inc.

#ifndef IPOKE_STORE_H
#define IPOKE_STORE_H

#include "PokeBox.h"
#include <list>
#include <memory>
#include <string>

namespace Server
{

    /// Represents a Pokémon storage system.
    class IPokeStore
    {
    public:
        virtual ~IPokeStore() = default;

        /// Saves the Pokémon collection for a specific user.
        /// @param userId The user ID.
        /// @param pokemon The Pokémon collection to save.
        virtual void saveCollection(const std::string& userId, CatchThemAll::PokemonList pokemon) = 0;

        /// Retrieves the Pokémon collection for a specific user.
        /// @param userId The user ID.
        /// @returns The saved Pokémon collection, or an empty list if no collection was saved for @p userId.
        virtual CatchThemAll::PokemonList retrieveCollection(const std::string& userId) const = 0;
    };

    using IPokeStorePtr = std::shared_ptr<IPokeStore>;
}
#endif
