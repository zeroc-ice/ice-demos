# C# Demos


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
dotnet restore "C# demos.sln"
dotnet msbuild "C# demos.sln" /p:IceToolsPath=<Ice dist path>/cpp/bin
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
