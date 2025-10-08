// Copyright (c) ZeroC, Inc.

#include <Glacier2/Session.ice>

module CatchThemAll
{
    /// Represents a list of Pokémons.
    sequence<string> PokemonList;

    /// Represents the Pokémons collected by a user.
    interface PokePen
    {
        /// Lists all the Pokémons in this pen.
        /// @return The list of Pokémons.
        ["cs:identifier:GetInventory"]
        PokemonList getInventory();

        /// Adds one or more Pokémons to this pen.
        /// @param pokemons The Pokémons to add.
        ["cs:identifier:Caught"]
        void caught(PokemonList pokemons);

        /// Releases all the Pokémons.
        ["cs:identifier:ReleaseAll"]
        void releaseAll();
    }

    interface PokeSession : Glacier2::Session
    {
        /// Retrieves the PokePen proxy associated with this session.
        /// @return The PokePen proxy.
        ["cs:identifier:GetPokePen"]
        PokePen* getPokePen();
    }
}
