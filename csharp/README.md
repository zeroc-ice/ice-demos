# C# Demos

- [Overview](#overview)
- [Building and running the demos](#building-and-running-the-demos)

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

Refer to the [C++11 demos](../cpp11) for more examples that use the Ice services
(Glacier2, IceGrid, IceStorm).

## Building and running the demos

### Build Requirements

In order to build Ice for .NET sample programs, you need all of the following:

 - the [.NET SDK][5] version 5.0
 - Slice to C# compiler, for Windows platform the Slice to C# compiler is included
   with the `zeroc.ice.net` NuGet package installed during build, for other platforms
   refer to the [Release Notes][6].

You can build from the command-line or with Visual Studio (2017 or 2019, for
Windows or Mac). If you use Visual Studio, the [Ice Builder][3] extension is
optional on Windows, and not available on macOS.

### Building the demos using NuGet packages:

Open a command prompt and change to the `csharp` subdirectory:

```
cd csharp
```

To build the sample programs run:

```
dotnet restore "C# demos.sln"
dotnet msbuild "C# demos.sln"
```

The demos are configured to use the Ice NuGet packages. These packages are automatically
downloaded from nuget.org during the build.

### Building the demos using a source build:

Open a command prompt and change to the `csharp` subdirectory:

```
cd csharp
```

Install packages from the source build:

On Windows

```
msbuild /p:IceHome=<Ice dist path> /t:InstallLocalPackages msbuild\ice.proj
```

On Linux and macOS

```
dotnet msbuild /p:IceHome=<Ice dist path> /t:InstallLocalPackages msbuild/ice.proj
```

To build the sample programs run:

```
dotnet restore "C# NET demos.sln"
dotnet msbuild "C# NET demos.sln"
```

### Running the Demos

For most demos, you can simply run `dotnet run --project server\server.csproj` and
`dotnet run --project client\client.csproj` in separate command prompt windows. Refer to the README.md file in each demo
directory for the exact usage instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not included in the `zeroc.ice.net` NuGet package.
To run these demos, the simplest is to first install the Ice binary distribution for your platform  and add its bin
directory to your PATH. Please refer to the [Release Notes][6] for additional information.

[1]: https://doc.zeroc.com/ice/4.0/introduction
[2]: https://doc.zeroc.com/technical-articles/general-topics/chat-demo
[3]: https://marketplace.visualstudio.com/items?itemName=ZeroCInc.IceBuilder
[4]: https://doc.zeroc.com/ice/4.0/release-notes/using-the-windows-binary-distributions
[5]: https://dotnet.microsoft.com/download/dotnet-core
[6]: https://doc.zeroc.com/rel/ice-releases/ice-4-0/ice-4-0-0-release-notes
