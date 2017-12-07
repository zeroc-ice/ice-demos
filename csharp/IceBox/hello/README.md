To run this demo, open two Command Prompt windows.

In the first window, start the IceBox server for .NET.

### .NET Framework 4.5

The IceBox server for .NET Framework 4.5 (`iceboxnet.exe`) is in the `tools\net45` folder
of `zeroc.ice.net` NuGet package. The file is copied to the demo directory during the build.

Then start IceBox:
```
iceboxnet --Ice.Config=config.icebox
```

### .NET Core 2.0

The IceBox server for .NET Core 2.0 (`iceboxnet.dll`) is in the `tools\netcoreapp2.0` folder
of `zeroc.ice.net` NuGet package. The file is copied to the demo directory during the build.

Start IceBox:

```
dotnet iceboxnet.dll --Ice.Config=config.icebox
```

In the second window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |
