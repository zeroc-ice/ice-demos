// Copyright (c) ZeroC, Inc. All rights reserved.

using Filesystem;
using System;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement
await using var communicator = new Communicator(ref args);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) =>
    {
        eventArgs.Cancel = true;
        _ = communicator.ShutdownAsync();
    };

// Create an object adapter.
ObjectAdapter adapter = communicator.CreateObjectAdapterWithEndpoints("SimpleFilesystem",
                                                                      "ice+tcp://localhost:10000");

// Create the root directory (with name "/" and no parent)
var root = new Directory("/", null);
root.ActivateAsync(adapter);

// Create a file called "README" in the root directory
var file = new File(
    "README",
    root,
    new string[] { "This file system contains a collection of poetry." });

file.ActivateAsync(adapter);

// Create a directory called "Coleridge" in the root directory
var coleridge = new Directory("Coleridge", root);
coleridge.ActivateAsync(adapter);

// Create a file called "Kubla_Khan" in the Coleridge directory
file = new File(
    "Kubla_Khan",
    coleridge,
    new string[]{ "In Xanadu did Kubla Khan",
                  "A stately pleasure-dome decree:",
                  "Where Alph, the sacred river, ran",
                  "Through caverns measureless to man",
                  "Down to a sunless sea." });
file.ActivateAsync(adapter);

// All objects are created, allow client requests now
await adapter.ActivateAsync();

// Wait until we are done
communicator.WaitForShutdownAsync();
