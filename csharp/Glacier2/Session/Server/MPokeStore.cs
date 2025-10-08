// Copyright (c) ZeroC, Inc.

using System.Collections.Immutable;

namespace Server;

/// <summary>In-memory implementation of <see cref="IPokeStore"/>.</summary>
internal class MPokeStore : IPokeStore
{
    private readonly Dictionary<string, ImmutableList<string>> _store = [];

    /// <inheritdoc />
    public void SaveCollection(string userId, ICollection<string> pokemons)
    {
        if (pokemons.Count == 0)
        {
            _store.Remove(userId);
        }
        else
        {
            _store[userId] = [.. pokemons];
        }
    }

    /// <inheritdoc />
    public ICollection<string>? RetrieveCollection(string userId) =>
        _store.TryGetValue(userId, out ImmutableList<string>? pokemons) ? pokemons : null;
}
