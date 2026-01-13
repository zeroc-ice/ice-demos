# C# Demos

- [C# Demos](#c-demos)
  - [Overview](#overview)
  - [Building and running the Demos with .NET on Windows, Linux, and macOS](#building-and-running-the-demos-with-net-on-windows-linux-and-macos)
    - [Build Requirements](#build-requirements)
    - [Building the demos](#building-the-demos)
    - [Running the Demos](#running-the-demos)

## Overview

This directory contains C# sample programs for various Ice components. These examples are provided to get you started
using a particular Ice feature or coding technique.

Most of the subdirectories here correspond directly to Ice components, such as [Glacier2](./Glacier2),
[IceBox](./IceBox), and so on. We've also included the [Manual](./Manual) subdirectory with complete examples for
some of the code snippets in the [Ice manual].

Refer to the [C++11 demos](../cpp11) for more examples that use the Ice services (Glacier2, IceGrid, IceStorm).

## Building and running the Demos with .NET on Windows, Linux, and macOS

### Build Requirements

To build the Ice for .NET samples, you need all of the following:

- the [.NET SDK] version 8.0 or later
- the Slice to C# compiler
  - on Windows, it is included with the `zeroc.ice.net` NuGet package installed during the build
  - on other platforms, see the [Release Notes]

### Building the demos

Open a command prompt and change to the `csharp` subdirectory:

```shell
cd csharp
```

To build the sample programs run:

```shell
dotnet restore "C# NET demos.sln"
dotnet msbuild "C# NET demos.sln"
```

The demos are configured to use the Ice NuGet packages. These packages are automatically downloaded from nuget.org
during the build.

### Running the Demos

For most demos, you can run `server` and `client` in separate command prompt windows. Refer to the README.md file in
each demo directory for the exact usage instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not included in the `zeroc.ice.net` NuGet package.
To run these demos, the simplest approach is to install the Ice binary distribution for your platform and add its
bin directory to your PATH. Please refer to the [Release Notes] for additional information.

[Ice manual]: https://doc.zeroc.com/ice/3.7/introduction
[.NET SDK]: https://download/dotnet/8.0
[Release Notes]: https://doc.zeroc.com/rel/ice-releases/ice-3-7/ice-3-7-10-release-notes
