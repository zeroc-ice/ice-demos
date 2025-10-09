// Copyright (c) ZeroC, Inc.

namespace Server;

/// <summary>SessionManager is an Ice servant that implements Slice interface Glacier2::SessionManager. It creates
/// sessions and resolves user IDs from session tokens.</summary>
internal class SessionManager : Glacier2.SessionManagerDisp_, IUserIdResolver
{
    private readonly Ice.ObjectAdapter _sessionAdapter;
    private readonly Dictionary<string, string> _tokenToUserId = [];

    /// <inheritdoc />
    public override Glacier2.SessionPrx? create(
        string userId,
        Glacier2.SessionControlPrx? sessionControl,
        Ice.Current current)
    {
        if (sessionControl is null)
        {
            Console.WriteLine("Error: set Glacier2.Server.Endpoints in the Glacier2 router configuration.");
            return null;
        }

        Ice.ObjectPrx proxy = _sessionAdapter.addWithUUID(
            new DefaultPokeSession(current.adapter, sessionControl, this));
        string sessionToken = proxy.ice_getIdentity().name;
        _tokenToUserId[sessionToken] = userId;

        Console.WriteLine($"Creating session #{sessionToken} for user '{userId}'");
        return Glacier2.SessionPrxHelper.uncheckedCast(proxy);
    }

    /// <inheritdoc />
    public string? GetUserId(string token) => _tokenToUserId.TryGetValue(token, out string? userId) ? userId : null;

    /// <inheritdoc />
    public void RemoveToken(string token) => _tokenToUserId.Remove(token);

    /// <summary>Constructs a SessionManager servant.</summary>
    /// <param name="adapter">The object adapter that hosts the session servants.</param>
    internal SessionManager(Ice.ObjectAdapter adapter) =>
        _sessionAdapter = adapter;
}
