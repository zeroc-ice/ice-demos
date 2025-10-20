// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>Represents a Pokémon storage system.</summary>
internal interface IPokeStore
{
    /// <summary>Saves the Pokémon collection for a specific user.</summary>
    /// <param name="userId">The user's ID.</param>
    /// <param name="pokemon">The Pokémon collection to save.</param>
    void SaveCollection(string userId, IReadOnlyCollection<string> pokemon);

    /// <summary>Retrieves the Pokémon collection for a specific user.</summary>
    /// <param name="userId">The user's ID.</param>
    /// <returns>The saved Pokémon collection, or null if no collection was saved for <paramref name="userId"/>.
    /// </returns>
    IReadOnlyCollection<string>? RetrieveCollection(string userId);
}
