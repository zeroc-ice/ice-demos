# C# Demos

- [C# Demos](#c-demos)
  - [Overview](#overview)
  - [Building Requirements](#building-requirements)
  - [Building the Demos](#building-the-demos)

## Overview

This directory contains C# sample programs demonstrating how to use various features of
[Ice](https://zeroc.com/products/ice) framework.

Each subdirectory includes a small demo focusing on a specific Ice component or coding technique.

## Building Requirements

1. **.NET SDK 8.0**
   Download and install the .NET 8.0 SDK from
   [dotnet.microsoft.com](https://dotnet.microsoft.com/en-us/download/dotnet).

2. **ZeroC NuGet Feed**
   The included [nuget.config](./nuget.config) file already adds the ZeroC NuGet feed to make
   the ZeroC nightly builds available.

## Building the Demos

The top-level [Directory.Build.props](./Directory.Build.props) file sets `IceVersion` to
`3.8.0-nightly.*` by default to pull the latest nightly build of Ice.
You can override this on the command line:

```shell
dotnet build -p:IceVersion="3.8.0~alpha0"
```

Check each demo’s README for detailed instructions on building and running that specific example.
