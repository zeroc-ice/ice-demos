// Copyright (c) ZeroC, Inc.

using System.Collections.Immutable;

namespace Server;

/// <summary>An in-memory implementation of <see cref="IPokeStore"/>.</summary>
/// <remarks>This mock implementation is not thread-safe. This is not a concern for this demo since there is only one
/// thread in the Ice server thread pool. However, a real implementation should support concurrent calls.</remarks>
internal class InMemoryPokeStore : IPokeStore
{
    private readonly Dictionary<string, ImmutableList<string>> _store = [];

    /// <inheritdoc />
    public void SaveCollection(string userId, ICollection<string> pokemon)
    {
        if (pokemon.Count == 0)
        {
            _store.Remove(userId);
        }
        else
        {
            _store[userId] = [.. pokemon];
        }
    }

    /// <inheritdoc />
    public ICollection<string>? RetrieveCollection(string userId) =>
        _store.TryGetValue(userId, out ImmutableList<string>? pokemon) ? pokemon : null;
}
