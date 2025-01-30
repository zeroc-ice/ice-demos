# C# Demos

- [C# Demos](#c-demos)
  - [Overview](#overview)
  - [Building and running the Demos](#building-and-running-the-demos)
    - [Build Requirements](#build-requirements)
    - [Building the demos](#building-the-demos)
    - [Building the demos using a source build](#building-the-demos-using-a-source-build)
    - [Running the Demos](#running-the-demos)

## Overview

This directory contains C# sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[Glacier2](./Glacier2), [IceBox](./IceBox), and so on. We've also included the
following additional subdirectories:

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

- [Chat](./Chat) contains a .NET GUI client for the ZeroC [Chat Demo][2].

Refer to the [C++ demos)(../cpp) for more examples that use the Ice services
(Glacier2, IceGrid, IceStorm).

## Building and running the Demos

### Build Requirements

In order to build Ice for .NET sample programs, you need all of the following:

- the [.NET SDK][3] version 8.0 or later

You can build from the command-line or with Visual Studio 2022 for Windows.

### Building the demos

Open a command prompt and change to the `csharp` subdirectory:

```shell
cd csharp
```

To build the sample programs run:

```shell
dotnet build "C# NET demos.sln"
```

For building the  demos from the `main` branch you need to first install the .NET Ice NuGet packages, see the next
section for details.

### Building the demos using a source build

Refer to the C# build instructions for creating and publishing the NuGet packages from a source build:

https://github.com/zeroc-ice/ice/blob/main/csharp/BUILDING.md#nuget-package

### Running the Demos

For most demos, you can simply run `server` and `client` in separate Command Prompt windows. Refer to the README.md
file in each demo directory for the exact usage instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not included in the `zeroc.ice.net` NuGet
package. To run these demos, the simplest is to first install the Ice binary distribution for your platform and add its
bin directory to your PATH. Please refer to the [Release Notes][4] for additional information.

[1]: https://doc.zeroc.com/ice/3.7/introduction
[2]: https://doc.zeroc.com/technical-articles/general-topics/chat-demo
[3]: https://download/dotnet/8.0
[4]: https://doc.zeroc.com/rel/ice-releases/ice-3-7/ice-3-7-10-release-notes
