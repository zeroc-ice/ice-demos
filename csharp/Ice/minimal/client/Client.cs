// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using ZeroC.Ice;

using var communicator = new Communicator(ref args);
var hello = IHelloPrx.Parse("ice+tcp://localhost:10000/hello", communicator);
hello.SayHello();
