// Copyright (c) ZeroC, Inc.

import Foundation
import Glacier2
import Ice

// All the Pokemon we know about.
let allPokemon = [
    "Bulbasaur", "Ivysaur", "Venusaur", "Charmander", "Charmeleon", "Charizard", "Squirtle", "Wartortle", "Blastoise",
    "Caterpie", "Metapod", "Butterfree", "Weedle", "Kakuna", "Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Rattata",
    "Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Raichu", "Sandshrew", "Sandslash", "Nidoran♀",
    "Nidorina", "Nidoqueen", "Nidoran♂", "Nidorino", "Nidoking", "Clefairy", "Clefable", "Vulpix", "Ninetales",
    "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras", "Parasect", "Venonat",
    "Venomoth", "Diglett", "Dugtrio", "Meowth", "Persian", "Psyduck", "Golduck", "Mankey", "Primeape",
]

// Retrieve the user ID for this run.
let userId = CommandLine.arguments.count > 1 ? CommandLine.arguments[1] : NSUserName()

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
let communicator = try Ice.initialize(CommandLine.arguments)
defer {
    communicator.destroy()
}

// Create a proxy to the Glacier2 router. The addressing information (transport, host, and port number) is derived
// from the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
let router = try makeProxy(
    communicator: communicator, proxyString: "Glacier2/router:tcp -h localhost -p 4063",
    type: Glacier2.RouterPrx.self)

// Set this proxy as the default router for all future proxies created from this communicator.
communicator.setDefaultRouter(router)

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime
// of the session is the same as the lifetime of the connection.
// We configured a SessionManager on the Glacier2 router, so session is a non-nil PokeSession.
guard let session = try await router.createSession(userId: userId, password: "password") else {
    fatalError("createSession returned nil")
}

let pokeSession = uncheckedCast(prx: session, type: PokeSessionPrx.self)

// Retrieve the PokeBox proxy from the session.
guard let pokeBox = try await pokeSession.getPokeBox() else {
    fatalError("getPokeBox returned nil")
}

let currentCount = (try await pokeBox.getInventory()).count
print("\(userId)'s PokeBox contains \(currentCount) Pokémon.")

// Catch a few Pokémon.
let addCount = Int.random(in: 1...6)
print("Catching \(addCount) Pokémon...")
var newPokemon: [String] = []
for _ in 0..<addCount {
    newPokemon.append(allPokemon.randomElement()!)
}
try await pokeBox.caught(newPokemon)

// Display the contents of the PokeBox.
let inventory = try await pokeBox.getInventory()
print("\(userId)'s PokeBox now holds \(inventory.count) Pokémon:")
for pokemon in inventory {
    print("\t\(pokemon)")
}

if inventory.count > 10 {
    print("Oh no! All the Pokémon escaped!")
    try await pokeBox.releaseAll()
}

// Exiting, closing the connection, or calling destroy on the session terminates both PokeSession and the
// internal session state maintained by the Glacier2 router.
print("Destroying the session...")
try await pokeSession.destroy()

// Verify the proxy no longer works.
do {
    _ = try await pokeBox.getInventory()
    print("Error: the PokeBox proxy should not work without a session!")
} catch is Ice.ConnectionLostException {
    // We get a ConnectionLostException because the Glacier2 router aborts the request on the (new) connection
    // without an associated session.
    print("The PokeBox proxy is no longer valid, as expected.")
}

// Create a new session. This allows us to reach the PokeBox object again.
print("Creating a new session...")
guard try await router.createSession(userId: userId, password: "password") != nil else {
    fatalError("createSession returned nil")
}

do {
    // The pokeBox proxy no longer works as it was created with the token of an old session.
    _ = try await pokeBox.getInventory()
    print("Error: the PokeBox proxy should not work with a new session!")
} catch let error as Ice.DispatchException {
    if error.replyStatus == ReplyStatus.unauthorized.rawValue {
        // See code in SharedPokeBox.getUserId.
        print("The PokeBox proxy remains unusable, as expected.")
    } else {
        throw error
    }
}
