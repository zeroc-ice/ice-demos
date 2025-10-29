// Copyright (c) ZeroC, Inc.

#include "IPokeStore.h"
#include "IUserIdResolver.h"
#include "PokeBox.h"

#include <string>
#include <vector>

namespace Server
{

    /// SharedPokeBox is an Ice servant that implements Slice interface PokeBox. The same shared servant
    /// implements all PokeBox objects; this is doable because all the state is stored in the IPokeStore.
    class SharedPokeBox : public CatchThemAll::PokeBox
    {
    public:
        /// <inheritdoc />
        // Retrieve the Pokémon collection for the user associated with the current session.
        CatchThemAll::PokemonList getInventory(const Ice::Current& current) final;

        /// <inheritdoc />
        // Add new Pokémon to the Pokémon collection for the user associated with the current session.
        void caught(CatchThemAll::PokemonList pokemon, const Ice::Current& current) final;

        /// <inheritdoc />
        void releaseAll(const Ice::Current& current) final;

        /// <summary>Constructs a SharedPokeBox servant.</summary>
        /// <param name="pokeStore">The Poke store.</param>
        /// <param name="userIdResolver">The user ID resolver.</param>
        SharedPokeBox(IPokeStorePtr pokeStore, IUserIdResolverPtr userIdResolver);

    private:
        /// <summary>Retrieves the user ID associated with the current session.</summary>
        /// <param name="current">Information about the incoming request being dispatched.</param>
        std::string getUserId(const Ice::Current& current) const;

        const IUserIdResolverPtr _userIdResolver;
        const IPokeStorePtr _pokeStore;
    };

}
