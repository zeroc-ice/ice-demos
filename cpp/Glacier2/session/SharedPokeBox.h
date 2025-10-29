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
        /// Constructs a SharedPokeBox servant.
        /// @param pokeStore The Poke store.
        /// @param userIdResolver The user ID resolver.
        SharedPokeBox(IPokeStorePtr pokeStore, IUserIdResolverPtr userIdResolver);

        // Retrieve the Pokémon collection for the user associated with the current session.
        CatchThemAll::PokemonList getInventory(const Ice::Current& current) final;

        // Add new Pokémon to the Pokémon collection for the user associated with the current session.
        void caught(CatchThemAll::PokemonList pokemon, const Ice::Current& current) final;

        // Release all Pokémon from the Pokémon collection for the user associated with the current session.
        void releaseAll(const Ice::Current& current) final;

    private:
        /// Retrieves the user ID associated with the current session.
        /// @param current Information about the incoming request being dispatched.
        std::string getUserId(const Ice::Current& current) const;

        const IUserIdResolverPtr _userIdResolver;
        const IPokeStorePtr _pokeStore;
    };

}
