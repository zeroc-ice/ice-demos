# Xamarin Demos

## Overview

This directory contains a Xamarin client application for the Ice hello
demo.

### Building on Windows

#### Windows Build Requirements

* Visual Studio 2017 with following workloads:
  * Universal Windows Platform development
  * Mobile development with .NET
  * .NET Core cross-platform development

#### Building

Open the solution file `Xamarin demos.sln` using Visual Studio 2017 and
build the desired project `hello.Android`, `hello.iOS` or `hello.UWP`.

#### Running

To run the demo, first you need to start an Ice hello server from another
language mapping (Java, C++, C#, or Python). Please refer to the README
in the server subdirectory for more information on starting the server.
If you want to get started quickly, we recommend using the Python server.

The Xamarin Hello client application can be deploy to Android, iOS or UWP
using the `hello.Android`, `hello.iOS` and `hello.UWP` project `Deploy`
option respectivelly.

### Building on macOS

#### macOS Build Requirements

* Visual Studio 2017 for Mac

#### Building

Open the solution file `Xamarin demos.sln` using Visual Studio 2017 for Mac
and build the desired project `hello.Android` or `hello.iOS`.

#### Running

To run the demo, first you need to start an Ice hello server from another
language mapping (Java, C++, C#, or Python). Please refer to the README
in the server subdirectory for more information on starting the server.
If you want to get started quickly, we recommend using the Python server.

After starting the server open the solution file `Xamarin demos.sln` using
Visual Studio 2017 for Mac.

The Xamarin Hello client application can be deploy to Android or iOS
using the `hello.Android` and `hello.iOS` `Deploy` option respectivelly.
