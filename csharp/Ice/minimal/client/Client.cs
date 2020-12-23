// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using ZeroC.Ice;

await using var communicator = new Communicator(ref args);
await communicator.ActivateAsync();
var hello = IHelloPrx.Parse("ice+tcp://localhost:10000/hello", communicator);
hello.SayHello();
