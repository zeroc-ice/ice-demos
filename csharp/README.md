# C# Demos

- [C# Demos](#c-demos)
  - [Overview](#overview)
  - [Building and running the Demos with .NET on Windows, Linux, and macOS](#building-and-running-the-demos-with-net-60-on-windows-linux-and-macos)
    - [Build Requirements](#build-requirements)
    - [Building the demos using NuGet packages](#building-the-demos-using-nuget-packages)
    - [Building the demos using a source build](#building-the-demos-using-a-source-build)
    - [Running the Demos](#running-the-demos)
  - [Building and running the Demos with .NET Framework on Windows](#building-and-running-the-demos-with-net-framework-on-windows)
    - [Build Requirements](#build-requirements-1)
    - [Building the demos using NuGet packages](#building-the-demos-using-nuget-packages-1)
    - [Building the demos using a source build](#building-the-demos-using-a-source-build-1)
    - [Running the Demos](#running-the-demos-1)

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

## Building and running the Demos with .NET on Windows, Linux, and macOS

### Build Requirements

In order to build Ice for .NET sample programs, you need all of the following:

- the [.NET Core SDK][5] version 6.0 or later
- Slice to C# compiler, for Windows platform the Slice to C# compiler is included
  with the `zeroc.ice.net` NuGet package installed during build, for other platforms
  refer to the [Release Notes][6].

You can build from the command-line or with Visual Studio (2022 for Windows or Mac). If you use Visual Studio,
the [Ice Builder][3] extension is optional on Windows, and not available on macOS.

### Building the demos using NuGet packages

Open a command prompt and change to the `csharp` subdirectory:

```shell
cd csharp
```

To build the sample programs run:

```shell
dotnet restore "C# NET demos.sln"
dotnet msbuild "C# NET demos.sln"
```

The demos are configured to use the Ice NuGet packages. These packages are automatically
downloaded from nuget.org during the build.

### Building the demos using a source build

Open a command prompt and change to the `csharp` subdirectory:

```shell
cd csharp
```

Install packages from the source build:

On Windows

```shell
msbuild /p:IceHome=<Ice dist path> /t:NetInstallLocalPackages msbuild\ice.proj
```

On Linux and macOS

```shell
dotnet msbuild /p:IceHome=<Ice dist path> /t:NetInstallLocalPackages msbuild/ice.proj
```

To build the sample programs run:

```shell
dotnet restore "C# NET demos.sln"
dotnet msbuild "C# NET demos.sln"
```

### Running the Demos

For most demos, you can simply run `server` and `client` in separate Command
Prompt windows.  Refer to the README.md file in each demo directory for the
exact usage instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not
included in the `zeroc.ice.net` NuGet package. To run these demos, the simplest
is to first install the Ice binary distribution for your platform  and add its bin
directory to your PATH. Please refer to the [Release Notes][6] for additional information.

## Building and running the Demos with .NET Framework on Windows

### Build Requirements

In order to build the Ice for .NET sample programs, you need:

- Visual Studio
- the [Ice Builder][3] Visual Studio extension

If you forget to install Ice Builder, the C# sample projects will fail to load and
Visual Studio may give an error message that suggests a version migration problem:
> Unsupported
>
> This version of Visual Studio is unable to open the following projects. The project types may not be installed or this version of Visual Studio may not support them.
> For more information on enabling these project types or otherwise migrating your assets, please see the details in the "Migration Report" displayed after clicking OK.

### Building the demos using NuGet packages

Open the solution file `C# NET Framework demos.sln` to build the sample programs.

The demos are configured to use the Ice distribution from NuGet packages. These
packages are automatically downloaded during the build. If you have disabled the
automatic download of NuGet packages in Visual Studio, you need to restore the
packages before you build.

Packages can be restored via `Tools > NuGet Package Manager > Manage NuGet
Packages for Solution...` in Visual Studio.

After restoring the packages, right click on the desired demo in the Solution
Explorer window and select `Build`.

### Building the demos using a source build

- Build from command line:
  - Open a Visual Studio command prompt

    ```shell
    cd ice-demos\csharp
    MSBuild /p:IceHome=<Ice dist path> /t:Net45Build msbuild\ice.proj
    ```

- Build from Visual Studio:
  - Open a Visual Studio command prompt

    ```shell
    cd ice-demos\csharp
    MSBuild /p:IceHome=<Ice dist path> /t:Net45InstallLocalPackages msbuild\ice.proj
    ```

  - Open the solution file `C# NET Framework demos.sln` to build the sample programs.

### Running the Demos

For most demos, you can simply run `client` and `server` in separate Command
Prompt windows.  Refer to the README.md file in each demo directory for the
exact usage instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not
included in the `zeroc.ice.net` NuGet package. To run these demos, the simplest
is to first install the Ice MSI and add its bin directory to your PATH. Please
refer to [Using the Windows Binary Distributions][4] for additional information.

[1]: https://doc.zeroc.com/ice/3.7/introduction
[2]: https://doc.zeroc.com/technical-articles/general-topics/chat-demo
[3]: https://marketplace.visualstudio.com/items?itemName=ZeroCInc.IceBuilder
[4]: https://doc.zeroc.com/ice/3.7/release-notes/using-the-windows-binary-distributions
[5]: https://download/dotnet/6.0
[6]: https://doc.zeroc.com/rel/ice-releases/ice-3-7/ice-3-7-9-release-notes
