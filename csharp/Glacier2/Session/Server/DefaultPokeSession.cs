// Copyright (c) ZeroC, Inc.

using CatchThemAll;

namespace Server;

/// <summary>DefaultPokeSession is an Ice servant that implements Slice interface PokeSession.</summary>
internal class DefaultPokeSession : PokeSessionDisp_
{
    private readonly Ice.ObjectAdapter _adapter;
    private readonly Glacier2.SessionControlPrx _sessionControl;
    private readonly IUserIdResolver _userIdResolver;

    /// <inheritdoc />
    public override PokePenPrx? GetPokePen(Ice.Current current)
    {
        // The session token is the name component of the session identity; we use it for the identity of the PokePen
        // object as well.
        Ice.ObjectPrx proxy = _adapter.createProxy(new Ice.Identity(name: current.id.name, category: "pen"));
        return PokePenPrxHelper.uncheckedCast(proxy);
    }

    /// <inheritdoc />
    public override void destroy(Ice.Current current)
    {
        Console.WriteLine($"Destroying session #{current.id.name}");

        _userIdResolver.RemoveToken(current.id.name);

        // Remove this servant from the adapter.
        _adapter.remove(current.id);

        // Destroy the session in the Glacier2 router.
        _ = _sessionControl.destroyAsync();
    }

    /// <summary>Constructs a DefaultPokeSession servant.</summary>
    /// <param name="adapter">The object adapter that hosts this servant and the PokePen objects.</param>
    /// <param name="sessionControl">The session control proxy.</param>
    /// <param name="userIdResolver">The user ID resolver.</param>
    internal DefaultPokeSession(
        Ice.ObjectAdapter adapter,
        Glacier2.SessionControlPrx sessionControl,
        IUserIdResolver userIdResolver)
    {
        _adapter = adapter;
        _sessionControl = sessionControl;
        _userIdResolver = userIdResolver;
    }
}
