// Copyright (c) ZeroC, Inc. All rights reserved.

using Filesystem;
using System;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

// Create an object adapter.
ObjectAdapter adapter = communicator.CreateObjectAdapterWithEndpoints("SimpleFilesystem",
                                                                      "ice+tcp://localhost:10000");

// Create the root directory (with name "/" and no parent)
var root = new Directory("/", null);
root.Activate(adapter);

// Create a file called "README" in the root directory
var file = new File(
    "README",
    root,
    new string[] { "This file system contains a collection of poetry." });

file.Activate(adapter);

// Create a directory called "Coleridge" in the root directory
var coleridge = new Directory("Coleridge", root);
coleridge.Activate(adapter);

// Create a file called "Kubla_Khan" in the Coleridge directory
file = new File(
    "Kubla_Khan",
    coleridge,
    new string[]{ "In Xanadu did Kubla Khan",
                  "A stately pleasure-dome decree:",
                  "Where Alph, the sacred river, ran",
                  "Through caverns measureless to man",
                  "Down to a sunless sea." });
file.Activate(adapter);

// All objects are created, allow client requests now
adapter.Activate();

// Wait until we are done
communicator.WaitForShutdown();
