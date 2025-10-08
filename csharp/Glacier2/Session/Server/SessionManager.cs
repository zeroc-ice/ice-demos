// Copyright (c) ZeroC, Inc.

namespace Server;

internal class SessionManager : Glacier2.SessionManagerDisp_
{
    private readonly IPokeStore _pokeStore;

    /// <inheritdoc />
    public override Glacier2.SessionPrx? create(
        string userId,
        Glacier2.SessionControlPrx? sessionControl,
        Ice.Current current)
    {
        Console.WriteLine($"Creating session for user '{userId}'");

        // Create a PokePen servant for this user.
        PokePen pokePen = new(userId, _pokeStore);

        // Add the PokePen servant to the adapter with a UUID identity and return a proxy to the caller.
        return Glacier2.SessionPrxHelper.uncheckedCast(current.adapter.addWithUUID(pokePen));
    }

    /// <summary>Constructs a SessionManager servant.</summary>
    /// <param name="pokeStore">The poke store.</param>
    internal SessionManager(IPokeStore pokeStore)
    {
        _pokeStore = pokeStore;
    }
}
