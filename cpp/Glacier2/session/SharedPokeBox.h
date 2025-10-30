// Copyright (c) ZeroC, Inc.

#ifndef SHARED_POKE_BOX_H
#define SHARED_POKE_BOX_H

#include "PokeStore.h"
#include "UserIdResolver.h"
#include "PokeBox.h"

namespace Server
{
    /// SharedPokeBox is an Ice servant that implements Slice interface PokeBox. The same shared servant
    /// implements all PokeBox objects; this is doable because all the state is stored in the PokeStore.
    class SharedPokeBox : public CatchThemAll::PokeBox
    {
    public:
        /// Constructs a SharedPokeBox servant.
        /// @param pokeStore The Poke store.
        /// @param userIdResolver The user ID resolver.
        SharedPokeBox(PokeStorePtr pokeStore, UserIdResolverPtr userIdResolver);

        // Retrieve the Pokémon collection for the user associated with the current session.
        CatchThemAll::PokemonList getInventory(const Ice::Current& current) final;

        // Add new Pokémon to the Pokémon collection for the user associated with the current session.
        void caught(CatchThemAll::PokemonList pokemon, const Ice::Current& current) final;

        // Release all Pokémon from the Pokémon collection for the user associated with the current session.
        void releaseAll(const Ice::Current& current) final;

    private:
        /// Retrieves the user ID associated with the current session.
        /// @param current Information about the incoming request being dispatched.
        /// @returns The user ID associated with the current session.
        std::string getUserId(const Ice::Current& current) const;

        const PokeStorePtr _pokeStore;
        const UserIdResolverPtr _userIdResolver;
    };
}

#endif
