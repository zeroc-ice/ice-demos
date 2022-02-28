# Xamarin Demos

## Overview

This directory contains a Xamarin client application for the Ice hello
demo.

### Windows

#### Windows Build Requirements

* Visual Studio 2017 with following workloads:
  * Universal Windows Platform development
  * Mobile development with .NET
  * .NET Core cross-platform development

#### Building

Open the solution file `Xamarin demos.sln` using Visual Studio 2017 and
build the desired project `hello.Android`, `hello.iOS` or `hello.UWP`.

#### Running

You can deploy the Hello client application on a device or emulator, or run
it from Visual Studio with the debugger.

You also need to run a `hello` demo server provided by another language
mapping, such as C++ (cpp11, cpp98), C# (csharp) or Python (python).

### macOS

#### macOS Build Requirements

* Visual Studio 2017 for Mac

#### Building

Open the solution file `Xamarin demos.sln` using Visual Studio 2017 for Mac
and build the desired project `hello.Android` or `hello.iOS`.

#### Running

Press the Run button or select the `Run item` menu to run the Hello client on
the selected device or emulator.

You also need to run a `hello` demo server provided by another language
mapping, such as C++ (cpp11, cpp98), C# (csharp) or Python (python). Python is
the simplest as it does not require building anything.
