// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using ZeroC.Ice;

using var communicator = new Communicator(ref args);
var printer = IPrinterPrx.Parse("ice+tcp://localhost:10000/SimplePrinter", communicator);
printer.PrintString("Hello World!");
