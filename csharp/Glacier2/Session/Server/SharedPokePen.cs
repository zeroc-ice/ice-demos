// Copyright (c) ZeroC, Inc.

using CatchThemAll;

namespace Server;

/// <summary>SharedPokePen is an Ice servant that implements Slice interface PokePen. The same shared servant
/// implements all PokePen objects.</summary>
internal class SharedPokePen : PokePenDisp_
{
    private readonly IUserIdResolver _userIdResolver;
    private readonly IPokeStore _pokeStore;

    /// <inheritdoc />
    public override string[] GetInventory(Ice.Current current) =>
        _pokeStore.RetrieveCollection(GetUserId(current)) is ICollection<string> collection ? [.. collection] : [];

    /// <inheritdoc />
    public override void Caught(string[] pokemons, Ice.Current current)
    {
        string userId = GetUserId(current);
        SortedSet<string> savedPokemons =
            _pokeStore.RetrieveCollection(userId) is ICollection<string> collection ? new(collection) : [];
        savedPokemons.UnionWith(pokemons);
        _pokeStore.SaveCollection(userId, savedPokemons);
    }

    /// <inheritdoc />
    public override void ReleaseAll(Ice.Current current) =>
        _pokeStore.SaveCollection(GetUserId(current), []);

    /// <summary>Constructs a SharedPokePen servant.</summary>
    /// <param name="pokeStore">The poke store.</param>
    /// <param name="userIdResolver">The user ID resolver.</param>
    internal SharedPokePen(IPokeStore pokeStore, IUserIdResolver userIdResolver)
    {
        _pokeStore = pokeStore;
        _userIdResolver = userIdResolver;
    }

    private string GetUserId(Ice.Current current) =>
        _userIdResolver.GetUserId(current.id.name) ??
        throw new Ice.DispatchException(Ice.ReplyStatus.Unauthorized, "Invalid session token");
}
