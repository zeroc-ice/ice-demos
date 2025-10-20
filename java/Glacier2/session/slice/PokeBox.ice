// Copyright (c) ZeroC, Inc.

// Include the Glacier2/Session.ice file included in the slice-tools package.
#include <Glacier2/Session.ice>

["java:identifier:com.example.catchthemall"]
module CatchThemAll
{
    /// Represents a list of Pokémon.
    sequence<string> PokemonList;

    /// Represents the Pokémon collected by a user.
    interface PokeBox
    {
        /// Lists all the Pokémon in this box.
        /// @return The list of Pokémon.
        PokemonList getInventory();

        /// Adds one or more Pokémon to this box.
        /// @param pokemon The Pokémon to add.
        void caught(PokemonList pokemon);

        /// Releases all the Pokémon.
        void releaseAll();
    }

    /// Represents a specialized session for our Pokémon application.
    interface PokeSession : Glacier2::Session
    {
        /// Retrieves the PokeBox proxy associated with this session.
        /// @return The PokeBox proxy.
        PokeBox* getPokeBox();
    }
}
