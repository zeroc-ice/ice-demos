// Copyright (c) ZeroC, Inc.

import { Ice } from "@zeroc/ice";
import { VisitorCenter } from "./Greeter.js";
import process from "node:process";

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using communicator = new Ice.Communicator(process.argv);

// Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
// You can also set this proxy with the Ice.Default.Locator property.
communicator.setDefaultLocator(new Ice.LocatorPrx(communicator, "IceGrid/Locator:tcp -h localhost -p 4061"));

// Create a proxy to the Greeter object hosted by the server. 'greeter' is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
// The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to
// this client.
const greeter = new VisitorCenter.GreeterPrx(communicator, "greeter");

// Retrieve my name
const name = process.env.USER || process.env.USERNAME || "masked user";

// Send a request to the remote object and get the response.
let greeting = await greeter.greet(name);
console.log(greeting);

// Send another request to the remote object. With the default configuration we use for this client, this request
// reuses the connection and reaches the same server, even when we have multiple replicated servers.
greeting = await greeter.greet("alice");
console.log(greeting);
