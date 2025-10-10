// Copyright (c) ZeroC, Inc.

using CatchThemAll;

namespace Server;

/// <summary>DefaultPokeSession is an Ice servant that implements Slice interface PokeSession. We create a
/// DefaultPokeSession for each PokeSession object.</summary>
internal class DefaultPokeSession : PokeSessionDisp_
{
    // The object adapter that hosts this servant and the PokeBox objects.
    private readonly Ice.ObjectAdapter _adapter;

    // A proxy to the SessionControl object hosted by the Glacier2 router; this proxy allows us to control the Glacier2
    // session, in particular to destroy it.
    // In this demo, that's the only per-session state maintained by DefaultPokeSession.
    private readonly Glacier2.SessionControlPrx _sessionControl;

    private readonly IUserIdResolver _userIdResolver;

    /// <inheritdoc />
    public override PokeBoxPrx? GetPokeBox(Ice.Current current)
    {
        // The session token is the name component of the session identity; we use it for the identity of the PokeBox
        // object as well.
        Ice.ObjectPrx proxy = _adapter.createProxy(new Ice.Identity(name: current.id.name, category: "box"));
        return PokeBoxPrxHelper.uncheckedCast(proxy);
    }

    /// <inheritdoc />
    public override void destroy(Ice.Current current)
    {
        Console.WriteLine($"Destroying session #{current.id.name}");

        // Remove the token from the user ID resolver, since the token is no longer valid.
        _userIdResolver.RemoveToken(current.id.name);

        // Remove this servant from the object adapter. A new call to this session object will fail with
        // ObjectNotExistException.
        _adapter.remove(current.id);

        // Destroy the session in the Glacier2 router.
        _ = _sessionControl.destroyAsync();
    }

    /// <summary>Constructs a DefaultPokeSession servant.</summary>
    /// <param name="adapter">The object adapter that hosts this servant and the PokeBox objects.</param>
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
