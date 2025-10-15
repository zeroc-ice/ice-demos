// Copyright (c) ZeroC, Inc.

using CatchThemAll;
using System.Diagnostics;
using System.Security.Cryptography;

// All the Pokemon we know about.
string[] AllPokemon =
[
    "Bulbasaur", "Ivysaur", "Venusaur", "Charmander", "Charmeleon", "Charizard", "Squirtle", "Wartortle", "Blastoise",
    "Caterpie", "Metapod", "Butterfree", "Weedle", "Kakuna", "Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Rattata",
    "Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Raichu", "Sandshrew", "Sandslash", "Nidoran♀",
    "Nidorina", "Nidoqueen", "Nidoran♂", "Nidorino", "Nidoking", "Clefairy", "Clefable", "Vulpix", "Ninetales",
    "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras", "Parasect", "Venonat",
    "Venomoth", "Diglett", "Dugtrio", "Meowth", "Persian", "Psyduck", "Golduck", "Mankey", "Primeape"
];

// Retrieve the user ID for this run.
string userId = args.Length > 0 ? args[0] : Environment.UserName;

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the Glacier2 router. The addressing information (transport, host, and port number) is derived from
// the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
Glacier2.RouterPrx router =
    Glacier2.RouterPrxHelper.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

// Set this proxy as the default router for all future proxies created from this communicator.
communicator.setDefaultRouter(router);

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
// session is the same as the lifetime of the connection.
Glacier2.SessionPrx? session = await router.createSessionAsync(userId, "password");

// We configured a SessionManager on the Glacier2 router, so session is a non-null PokeSession.
PokeSessionPrx? pokeSession = PokeSessionPrxHelper.uncheckedCast(session);
Debug.Assert(pokeSession is not null);

// Retrieve the PokeBox proxy from the session.
PokeBoxPrx? pokeBox = await pokeSession.GetPokeBoxAsync();
Debug.Assert(pokeBox is not null);

int currentCount = (await pokeBox.GetInventoryAsync()).Length;
Console.WriteLine($"{userId}'s PokeBox contains {currentCount} Pokémon.");

// Catch a few Pokémon.
int addCount = RandomNumberGenerator.GetInt32(1, 6);
Console.Write($"Catching {addCount} Pokémon... ");
var newPokemon = new List<string>();
for (int i = 0; i < addCount; ++i)
{
    newPokemon.Add(AllPokemon[RandomNumberGenerator.GetInt32(AllPokemon.Length)]);
}
await pokeBox.CaughtAsync(newPokemon.ToArray());

// Display the contents of the PokeBox.
string[] inventory = await pokeBox.GetInventoryAsync();
Console.WriteLine($"{userId}'s PokeBox now holds {inventory.Length} Pokémon:");
foreach (string pokemon in inventory)
{
    Console.WriteLine($"\t{pokemon}");
}

if (inventory.Length > 10)
{
    Console.WriteLine("Oh no! All the Pokémon escaped!");
    await pokeBox.ReleaseAllAsync();
}

// Exiting, closing the connection, or calling destroyAsync on the session terminates both PokeSession and the
// internal session state maintained by the Glacier router.
Console.WriteLine("Destroying the session...");
await pokeSession.destroyAsync();

// Verify the proxy no longer works.
try
{
    _ = await pokeBox.GetInventoryAsync();
    Console.WriteLine("Error: the PokeBox proxy should not work without a session!");
}
catch (Ice.ConnectionLostException)
{
    // We get a ConnectionLostException because the Glacier2 router aborts the request on the (new) connection without
    // an associated session.
    Console.WriteLine("The PokeBox proxy is no longer valid, as expected.");
}

// Create a new session. This allows us to reach the PokeBox object again.
Console.WriteLine("Creating a new session...");
session = await router.createSessionAsync(userId, "password");

try
{
    // The pokeBox proxy no longer works as it was created with the token of an old session.
    _ = await pokeBox.GetInventoryAsync();
    Console.WriteLine("Error: the PokeBox proxy should not work with a new session!");
}
catch (Ice.DispatchException dispatchException) when (dispatchException.replyStatus == Ice.ReplyStatus.Unauthorized)
{
    // See code in SharedPokeBox.GetUserId.
    Console.WriteLine("The PokeBox proxy remains unusable, as expected.");
}
