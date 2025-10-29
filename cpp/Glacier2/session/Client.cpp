// Copyright (c) ZeroC, Inc.

#include "../../common/Env.h"
#include "PokeBox.h"

#include <Glacier2/Glacier2.h>
#include <Ice/Ice.h>
#include <array>
#include <cassert>
#include <iostream>
#include <random>

using namespace std;
using namespace CatchThemAll;

// All the Pokemon we know about.
std::array<const char*, 57> allPokemon = {
    "Bulbasaur", "Ivysaur",  "Venusaur",  "Charmander", "Charmeleon", "Charizard", "Squirtle",   "Wartortle",
    "Blastoise", "Caterpie", "Metapod",   "Butterfree", "Weedle",     "Kakuna",    "Beedrill",   "Pidgey",
    "Pidgeotto", "Pidgeot",  "Rattata",   "Raticate",   "Spearow",    "Fearow",    "Ekans",      "Arbok",
    "Pikachu",   "Raichu",   "Sandshrew", "Sandslash",  "Nidoran♀",   "Nidorina",  "Nidoqueen",  "Nidoran♂",
    "Nidorino",  "Nidoking", "Clefairy",  "Clefable",   "Vulpix",     "Ninetales", "Jigglypuff", "Wigglytuff",
    "Zubat",     "Golbat",   "Oddish",    "Gloom",      "Vileplume",  "Paras",     "Parasect",   "Venonat",
    "Venomoth",  "Diglett",  "Dugtrio",   "Meowth",     "Persian",    "Psyduck",   "Golduck",    "Mankey",
    "Primeape"};

int
main(int argc, char* argv[])
{
    // Retrieve the user ID for this run.
    string userId = argc > 1 ? argv[1] : Env::getUsername();

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy to the Glacier2 router. The addressing information (transport, host, and port number) is derived
    // from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
    Glacier2::RouterPrx router{communicator, "Glacier2/router:tcp -h localhost -p 4063"};

    // Set this proxy as the default router for all future proxies created from this communicator.
    communicator->setDefaultRouter(router);

    // Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
    // username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime
    // of the session is the same as the lifetime of the connection.
    optional<Glacier2::SessionPrx> session = router->createSession(userId, "password");

    // We configured a SessionManager on the Glacier2 router, so session is a non-null PokeSession.
    assert(session);

    PokeSessionPrx pokeSession = Ice::uncheckedCast<PokeSessionPrx>(*session);

    // Retrieve the PokeBox proxy from the session.
    optional<PokeBoxPrx> pokeBox = pokeSession->getPokeBox();
    assert(pokeBox);

    size_t currentCount = pokeBox->getInventory().size();
    cout << userId << "'s PokeBox contains " << currentCount << " Pokémon." << endl;

    // Catch a few Pokémon.
    // Initialize random number generators.
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> addDist{1, 6};
    std::uniform_int_distribution<> pokeDist{0, allPokemon.size() - 1};
    int addCount = addDist(gen);
    cout << "Catching " << addCount << " Pokémon... " << endl;
    vector<string> newPokemon;
    newPokemon.reserve(addCount);
    for (int i = 0; i < addCount; ++i)
    {
        newPokemon.push_back(allPokemon[pokeDist(gen)]);
    }
    pokeBox->caught(newPokemon);

    // Display the contents of the PokeBox.
    PokemonList inventory = pokeBox->getInventory();
    cout << userId << "'s PokeBox now holds " << inventory.size() << " Pokémon:" << endl;
    for (const string& pokemon : inventory)
    {
        cout << "\t" << pokemon << endl;
    }

    if (inventory.size() > 10)
    {
        cout << "Oh no! All the Pokémon escaped!" << endl;
        pokeBox->releaseAll();
    }

    // Exiting, closing the connection, or calling `destroy(Async)` on the session terminates both PokeSession and the
    // internal session state maintained by the Glacier2 router.
    cout << "Destroying the session..." << endl;
    pokeSession->destroy();

    // Verify the proxy no longer works.
    try
    {
        pokeBox->getInventory();
        cout << "Error: the PokeBox proxy should not work without a session!" << endl;
    }
    catch (const Ice::ConnectionLostException&)
    {
        // We get a ConnectionLostException because the Glacier2 router aborts the request on the (new) connection
        // without an associated session.
        cout << "The PokeBox proxy is no longer valid, as expected." << endl;
    }

    // Create a new session. This allows us to reach the PokeBox object again.
    cout << "Creating a new session..." << endl;
    session = router->createSession(userId, "password");

    try
    {
        // The pokeBox proxy no longer works as it was created with the token of an old session.
        pokeBox->getInventory();
        cout << "Error: the PokeBox proxy should not work with a new session!" << endl;
    }
    catch (const Ice::DispatchException& dispatchException)
    {
        if (dispatchException.replyStatus() == Ice::ReplyStatus::Unauthorized)
        {
            // See code in SharedPokeBox::getUserId.
            cout << "The PokeBox proxy remains unusable, as expected." << endl;
        }
        else
        {
            throw;
        }
    }

    return 0;
}
