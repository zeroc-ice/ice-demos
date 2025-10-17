// Copyright (c) ZeroC, Inc.

package com.example.glacier2.session.client;

import com.example.catchthemall.PokeBoxPrx;
import com.example.catchthemall.PokeSessionPrx;

import com.zeroc.Glacier2.CannotCreateSessionException;
import com.zeroc.Glacier2.PermissionDeniedException;
import com.zeroc.Glacier2.RouterPrx;
import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.ConnectionLostException;
import com.zeroc.Ice.DispatchException;
import com.zeroc.Ice.Util;

class Client {
    /** All the Pokemon we know about. */
    static final String[] ALL_POKEMON = {
        "Bulbasaur", "Ivysaur", "Venusaur", "Charmander", "Charmeleon", "Charizard", "Squirtle", "Wartortle",
        "Blastoise", "Caterpie", "Metapod", "Butterfree", "Weedle", "Kakuna", "Beedrill", "Pidgey", "Pidgeotto",
        "Pidgeot", "Rattata", "Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Raichu", "Sandshrew",
        "Sandslash", "Nidoran♀", "Nidorina", "Nidoqueen", "Nidoran♂", "Nidorino", "Nidoking", "Clefairy", "Clefable",
        "Vulpix", "Ninetales", "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras",
        "Parasect", "Venonat", "Venomoth", "Diglett", "Dugtrio", "Meowth", "Persian", "Psyduck", "Golduck", "Mankey",
        "Primeape"
    };

    public static void main(String[] args) {

        // Retrieve the user ID for this run.
        final String userId = args.length > 0 ? args[0] : System.getProperty("user.name");

        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = Util.initialize(args)) {

            // Create a proxy to the Glacier2 router. The addressing information (transport, host, and port number) is
            // derived from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
            RouterPrx router = RouterPrx.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

            // Set this proxy as the default router for all future proxies created from this communicator.
            communicator.setDefaultRouter(router);

            // Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
            // username/password combination. This call establishes a network connection to the Glacier2 router; the
            // lifetime of the session is the same as the lifetime of the connection.
            SessionPrx session;
            try {
                session = router.createSession(userId, "password");
            } catch (PermissionDeniedException | CannotCreateSessionException e) {
                System.out.println("Could not create session: " + e.getMessage());
                return;
            }

            // We configured a SessionManager on the Glacier2 router, so 'session' is a non-null 'PokeSession'.
            PokeSessionPrx pokeSession = PokeSessionPrx.uncheckedCast(session);
            assert pokeSession != null;

            // Retrieve the PokeBox proxy from the session.
            PokeBoxPrx pokeBox = pokeSession.getPokeBox();
            assert pokeBox != null;

            int currentCount = pokeBox.getInventory().length;
            System.out.println(userId + "'s PokeBox contains " + currentCount + " Pokémon.");

            // Catch a few Pokémon.
            var randomSource = new java.util.Random();
            int addCount = randomSource.nextInt(6) + 1;
            System.out.println("Catching " + addCount + " Pokémon... ");
            String[] newPokemon = randomSource.ints(addCount, 0, ALL_POKEMON.length)
                .mapToObj(i -> ALL_POKEMON[i])
                .toArray(String[]::new);
            pokeBox.caught(newPokemon);

            // Display the contents of the PokeBox.
            String[] inventory = pokeBox.getInventory();
            System.out.println(userId + "'s PokeBox now holds " + inventory.length + " Pokémon:");
            for (String pokemon : inventory) {
                System.out.println("\t" + pokemon);
            }

            if (inventory.length > 10) {
                System.out.println("Oh no! All the Pokémon escaped!");
                pokeBox.releaseAll();
            }

            // Exiting, closing the connection, or calling 'destroy' on the session terminates both
            // PokeSession and the internal session state maintained by the Glacier2 router.
            System.out.println("Destroying the session...");
            pokeSession.destroy();

            // Verify the proxy no longer works.
            try {
                pokeBox.getInventory();
                System.out.println("Error: the PokeBox proxy should not work without a session!");
            } catch (ConnectionLostException expected) {
                // We get a ConnectionLostException because the Glacier2 router aborts the request on the
                // (new) connection without an associated session.
                System.out.println("The PokeBox proxy is no longer valid, as expected.");
            }

            // Create a new session. This allows us to reach the PokeBox object again.
            System.out.println("Creating a new session...");
            try {
                session = router.createSession(userId, "password");
            } catch (PermissionDeniedException | CannotCreateSessionException e) {
                System.out.println("Could not create new session: " + e.getMessage());
                return;
            }

            try {
                // The pokeBox proxy no longer works as it was created with the token of an old session.
                pokeBox.getInventory();
                System.out.println("Error: the PokeBox proxy should not work with a new session!");
            } catch (DispatchException ex) {
                if (ex.replyStatus != com.zeroc.Ice.ReplyStatus.Unauthorized.value()) {
                    throw ex;
                }

                // See code in 'SharedPokeBox.getUserId'.
                System.out.println("The PokeBox proxy remains unusable, as expected.");
            }
        }
    }
}
