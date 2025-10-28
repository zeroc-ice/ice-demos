#!/usr/bin/env python
# Copyright (c) ZeroC, Inc.

import asyncio
import getpass
import random
import sys

# Slice module CatchThemAll in PokeBox.ice maps to Python module CatchThemAll.
import CatchThemAll
import Glacier2
import Ice

allPokemon = [
    "Bulbasaur",
    "Ivysaur",
    "Venusaur",
    "Charmander",
    "Charmeleon",
    "Charizard",
    "Squirtle",
    "Wartortle",
    "Blastoise",
    "Caterpie",
    "Metapod",
    "Butterfree",
    "Weedle",
    "Kakuna",
    "Beedrill",
    "Pidgey",
    "Pidgeotto",
    "Pidgeot",
    "Rattata",
    "Raticate",
    "Spearow",
    "Fearow",
    "Ekans",
    "Arbok",
    "Pikachu",
    "Raichu",
    "Sandshrew",
    "Sandslash",
    "Nidoran♀",
    "Nidorina",
    "Nidoqueen",
    "Nidoran♂",
    "Nidorino",
    "Nidoking",
    "Clefairy",
    "Clefable",
    "Vulpix",
    "Ninetales",
    "Jigglypuff",
    "Wigglytuff",
    "Zubat",
    "Golbat",
    "Oddish",
    "Gloom",
    "Vileplume",
    "Paras",
    "Parasect",
    "Venonat",
    "Venomoth",
    "Diglett",
    "Dugtrio",
    "Meowth",
    "Persian",
    "Psyduck",
    "Golduck",
    "Mankey",
    "Primeape",
]

# Retrieve the user ID for this run.
userId = sys.argv[1] if len(sys.argv) > 1 else getpass.getuser()


async def main():
    # Create an Ice communicator. We'll use this communicator to create proxies, and manage outgoing connections. We
    # enable asyncio support by passing the current event loop to initialize.
    async with Ice.Communicator(sys.argv, eventLoop=asyncio.get_running_loop()) as communicator:
        # Create a proxy to the Glacier2 router. The addressing information (transport, host, and port number) is
        # derived from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
        router = Glacier2.RouterPrx(communicator, "Glacier2/router:tcp -h localhost -p 4063")

        # Set this proxy as the default router for all future proxies created from this communicator.
        communicator.setDefaultRouter(router)

        # Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
        # username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
        # session is the same as the lifetime of the connection.
        session = await router.createSessionAsync(userId, "password")

        # We configured a SessionManager on the Glacier2 router, so session is a non-null PokeSession.
        pokeSession = CatchThemAll.PokeSessionPrx.uncheckedCast(session)
        assert pokeSession is not None

        # Retrieve the PokeBox proxy from the session.
        pokeBox = await pokeSession.getPokeBoxAsync()
        assert pokeBox is not None

        currentCount = len(await pokeBox.getInventoryAsync())
        print(f"{userId}'s PokeBox contains {currentCount} Pokémon.")

        # Catch a few Pokémon.
        addCount = random.randint(1, 6)
        print(f"Catching {addCount} Pokémon... ")
        newPokemon = [allPokemon[random.randint(0, len(allPokemon) - 1)] for _ in range(addCount)]
        await pokeBox.caughtAsync(newPokemon)

        # Display the contents of the PokeBox.
        inventory = await pokeBox.getInventoryAsync()
        print(f"{userId}'s PokeBox now holds {len(inventory)} Pokémon:")
        for pokemon in inventory:
            print(f"\t{pokemon}")

        if len(inventory) > 10:
            print("Oh no! All the Pokémon escaped!")
            await pokeBox.releaseAllAsync()

        # Exiting, closing the connection, or calling destroyAsync on the session terminates both PokeSession and the
        # internal session state maintained by the Glacier router.
        print("Destroying the session...")
        await pokeSession.destroyAsync()

        # Verify the proxy no longer works.
        try:
            _ = await pokeBox.getInventoryAsync()
            print("Error: the PokeBox proxy should not work without a session!")
        except Ice.ConnectionLostException:
            # We get a ConnectionLostException because the Glacier2 router aborts the request on the (new) connection without
            # an associated session.
            print("The PokeBox proxy is no longer valid, as expected.")

        # Create a new session. This allows us to reach the PokeBox object again.
        print("Creating a new session...")
        session = await router.createSessionAsync(userId, "password")

        try:
            # The pokeBox proxy no longer works as it was created with the token of an old session.
            _ = await pokeBox.getInventoryAsync()
            print("Error: the PokeBox proxy should not work with a new session!")
        except Ice.DispatchException as dispatchException:
            if dispatchException.replyStatus == Ice.ReplyStatus.Unauthorized.value:
                # See code in server.poke_session.box.SharedPokeBox.getUserId.
                print("The PokeBox proxy remains unusable, as expected.")


if __name__ == "__main__":
    asyncio.run(main())
