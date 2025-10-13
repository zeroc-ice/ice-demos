// Copyright (c) ZeroC, Inc.

// Include the Glacier/Session.ice file included in the Glacier2 NuGet package.
#include <Glacier2/Session.ice>

module CatchThemAll
{
    /// Represents a list of Pokémon.
    sequence<string> PokemonList;

    /// Represents the Pokémon collected by a user.
    interface PokeBox
    {
        /// Lists all the Pokémon in this box.
        /// @return The list of Pokémon.
        ["cs:identifier:GetInventory"]
        PokemonList getInventory();

        /// Adds one or more Pokémon to this box.
        /// @param pokemon The Pokémon to add.
        ["cs:identifier:Caught"]
        void caught(PokemonList pokemon);

        /// Releases all the Pokémon.
        ["cs:identifier:ReleaseAll"]
        void releaseAll();
    }

    /// Represents a specialized session for our Pokémon application.
    interface PokeSession : Glacier2::Session
    {
        /// Retrieves the PokeBox proxy associated with this session.
        /// @return The PokeBox proxy.
        ["cs:identifier:GetPokeBox"]
        PokeBox* getPokeBox();
    }
}
