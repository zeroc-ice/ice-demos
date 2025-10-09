// Copyright (c) ZeroC, Inc.

using CatchThemAll;

namespace Server;

/// <summary>SharedPokeBox is an Ice servant that implements Slice interface PokeBox. The same shared servant
/// implements all PokeBox objects; this is doable because all the state is stored in the IPokeStore.</summary>
internal class SharedPokeBox : PokeBoxDisp_
{
    private readonly IUserIdResolver _userIdResolver;
    private readonly IPokeStore _pokeStore;

    /// <inheritdoc />
    // Retrieve the Pokémon collection for the user associated with the current session.
    public override string[] GetInventory(Ice.Current current) =>
        _pokeStore.RetrieveCollection(GetUserId(current)) is ICollection<string> collection ? [.. collection] : [];

    /// <inheritdoc />
    // Add new Pokémon to the Pokémon collection for the user associated with the current session.
    public override void Caught(string[] pokemon, Ice.Current current)
    {
        string userId = GetUserId(current);

        List<string> savedPokemon =
            _pokeStore.RetrieveCollection(userId) is ICollection<string> collection ? new(collection) : [];

        savedPokemon.AddRange(pokemon);

        // Sort the collection to make it easier to see the Pokémon that have been caught.
        savedPokemon.Sort();
        _pokeStore.SaveCollection(userId, savedPokemon);
    }

    /// <inheritdoc />
    public override void ReleaseAll(Ice.Current current) =>
        _pokeStore.SaveCollection(GetUserId(current), []);

    /// <summary>Constructs a SharedPokeBox servant.</summary>
    /// <param name="pokeStore">The Poke store.</param>
    /// <param name="userIdResolver">The user ID resolver.</param>
    internal SharedPokeBox(IPokeStore pokeStore, IUserIdResolver userIdResolver)
    {
        _pokeStore = pokeStore;
        _userIdResolver = userIdResolver;
    }

    /// <summary>Retrieves the user ID associated with the current session.</summary>
    /// <param name="current">Information about the incoming request being dispatched.</param>
    private string GetUserId(Ice.Current current) =>
        _userIdResolver.GetUserId(current.id.name) ??
        throw new Ice.DispatchException(Ice.ReplyStatus.Unauthorized, "Invalid session token");
}
