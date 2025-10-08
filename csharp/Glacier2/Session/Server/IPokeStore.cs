// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Represents a Pokémon storage database.</summary>
internal interface IPokeStore
{
    /// <summary>Saves the Pokémon collection for a specific user.</summary>
    /// <param name="userId">The user ID.</param>
    /// <param name="pokemons">The Pokémon collection to save.</param>
    void SaveCollection(string userId, ICollection<string> pokemons);

    /// <summary>Retrieves the Pokémon collection for a specific user.</summary>
    /// <param name="userId">The user ID.</param>
    /// <returns>The saved Pokémon collection, or null if no collection was saved for <paramref name="userId"/>.
    /// </returns>
    ICollection<string>? RetrieveCollection(string userId);
}
