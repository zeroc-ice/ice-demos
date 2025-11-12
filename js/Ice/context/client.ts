// Copyright (c) ZeroC, Inc.

import { Ice } from "@zeroc/ice";
import { VisitorCenter } from "./Greeter.js";
import process from "node:process";

// Set the Ice.ImplicitContext property to "Shared" before calling Ice.Communicator constructor.
// This is only necessary for the implicit context API (see below).
const initData = new Ice.InitializationData();
initData.properties = new Ice.Properties(process.argv);
initData.properties.setProperty("Ice.ImplicitContext", "Shared");

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using communicator = new Ice.Communicator(initData);

// Create a Greeter proxy. If you run the server on a different computer, replace localhost in the string below with
// the server's hostname or IP address.
const greeter = new VisitorCenter.GreeterPrx(communicator, "greeter:tcp -h localhost -p 4061");

// Retrieve my name
const name = process.env.USER || process.env.USERNAME || "masked user";

// Send a request to the remote object and get the response. We request a French greeting by setting the context
// parameter.
let greeting = await greeter.greet(name, new Map([["language", "fr"]]));

console.log(greeting);

// Do it again, this time by setting the context on the proxy.
const greeterEs = greeter.ice_context(new Map([["language", "es"]]));

greeting = await greeterEs.greet("alice");
console.log(greeting);

// One more time, this time with an implicit context set on the communicator.
communicator.getImplicitContext().put("language", "de");
greeting = await greeter.greet("bob");
console.log(greeting);
