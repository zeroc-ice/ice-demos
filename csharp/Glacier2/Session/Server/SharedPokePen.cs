// Copyright (c) ZeroC, Inc.

using CatchThemAll;

namespace Server;

/// <summary>SharedPokePen is an Ice servant that implements Slice interface PokePen. The same shared servant
/// implements all PokePen objects.</summary>
internal class SharedPokePen : PokeCorralDisp_
{
    private readonly IUserIdResolver _userIdResolver;
    private readonly IPokeStore _pokeStore;

    /// <inheritdoc />
    public override string[] GetInventory(Ice.Current current)
    {
        string userId = _userIdResolver.GetUserId(current.id.name) ??
            throw new Ice.DispatchException(Ice.ReplyStatus.Unauthorized, "Invalid session token");

        return _pokeStore.RetrieveCollection(userId) is ICollection<string> collection ? [.. collection] : [];
    }

    /// <inheritdoc />
    public override void Caught(string[] pokemons, Ice.Current current) => _contents.UnionWith(pokemons);

    /// <inheritdoc />
    public override void ReleaseAll(Ice.Current current) => _contents.Clear();

    /// <summary>Constructs a PokePen servant.</summary>
    /// <param name="userId">The user ID.</param>
    /// <param name="pokeStore">The poke store.</param>
    internal SharedPokePen(IPokeStore pokeStore, IUserIdResolver)
    {
        _userId = userId;
        _pokeStore = pokeStore;
        _contents = _pokeStore.RetrieveCollection(userId) is ICollection<string> collection ? new(collection) : new();
    }
}
