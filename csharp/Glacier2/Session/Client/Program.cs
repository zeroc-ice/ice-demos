// Copyright (c) ZeroC, Inc.

using CatchThemAll;
using System.Diagnostics;
using System.Security.Cryptography;

// All the Pokémons we know about.
string[] AllPokemons =
[
    "Bulbasaur", "Ivysaur", "Venusaur", "Charmander", "Charmeleon", "Charizard", "Squirtle", "Wartortle", "Blastoise",
    "Caterpie", "Metapod", "Butterfree", "Weedle", "Kakuna", "Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Rattata",
    "Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Raichu", "Sandshrew", "Sandslash", "Nidoran♀",
    "Nidorina", "Nidoqueen", "Nidoran♂", "Nidorino", "Nidoking", "Clefairy", "Clefable", "Vulpix", "Ninetales",
    "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras", "Parasect", "Venonat",
    "Venomoth", "Diglett", "Dugtrio", "Meowth", "Persian", "Psyduck", "Golduck", "Mankey", "Primeape",
];

// Retrieve the user ID for this run.
string userId = args.Length > 0 ? args[0] : Environment.UserName;

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived from
// the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
Glacier2.RouterPrx router =
    Glacier2.RouterPrxHelper.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
// session is the same as the lifetime of the connection.
Glacier2.SessionPrx? session = await router.createSessionAsync(Environment.UserName, "password");

// Configure the proxy to route requests using the Glacier2 router.
session = Glacier2.SessionPrxHelper.uncheckedCast(session?.ice_router(router));

// We configured a SessionManager on the Glacier2 router, and the session it returns is our PokeSession.
PokeSessionPrx? pokeSession = PokeSessionPrxHelper.uncheckedCast(session);
Debug.Assert(pokeSession is not null);

// Retrieve the PokePen proxy from the session.
PokePenPrx? pokePen = await pokeSession.GetPokePenAsync();
Debug.Assert(pokePen is not null);

int count = (await pokePen.GetInventoryAsync()).Length;
Console.WriteLine($"{userId}'s PokePen contains {count} Pokémons.");

// Catch a few random Pokémons.
int newCount = RandomNumberGenerator.GetInt32(2, 6);
Console.Write($"Catching {newCount} Pokémons... ");
var newPokemons = new List<string>();
for (int i = 0; i < newCount; ++i)
{
    newPokemons.Add(AllPokemons[RandomNumberGenerator.GetInt32(AllPokemons.Length)]);
}
await pokePen.CaughtAsync(newPokemons.ToArray());

// Display the contents of the PokePen.
string[] currentPokemons = await pokePen.GetInventoryAsync();
Console.WriteLine($"{userId}'s PokePen now holds {currentPokemons.Length} Pokémons:");
foreach (string pokemon in currentPokemons)
{
    Console.WriteLine($"\t{pokemon}");
}

if (currentPokemons.Length > 10)
{
    Console.WriteLine("Oh no! All the Pokémons escaped!");
    await pokePen.ReleaseAllAsync();
}

await pokeSession.destroyAsync();

// Exiting, closing the connection, or calling destroyAsync on the session terminates both PokeSession and the session
// state maintained by the Glacier router.
