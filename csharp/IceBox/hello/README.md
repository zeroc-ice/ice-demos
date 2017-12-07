To run this demo, open two Command Prompt windows.

In the first window, start the IceBox server for .NET.

### .NET Framework 4.5

The IceBox server for .NET Framework 4.5 (`iceboxnet.exe`) is in the `tools\net45` folder
of `zeroc.ice.net` NuGet package. You should add this folder to your PATH with a command
similar to:

```
set PATH=%USERPROFILE%\ice-demos\csharp\packages\zeroc.ice.net.3.7.0\tools\net45;%PATH%
```

Then start IceBox:
```
iceboxnet --Ice.Config=config.icebox
```

### .NET Core 2.0

The IceBox server for .NET Core 2.0 (`iceboxnet.dll`) is in the `tools\netcoreapp2.0` folder
of `zeroc.ice.net` NuGet package. With .NET Core the package is installed in the NuGet global-packages
folder typically `%USERPROFILE%\.nuget\packages\` on Windows and `$HOME\.nuget\packages` on Linux.

On Windows:
```
dotnet %USERPROFILE%\.nuget\packages\zeroc.ice.net\3.7.0\tools\netcoreapp2.0\iceboxnet.dll --Ice.Config=config.icebox
```

On Linux:

```
dotnet $HOME\.nuget\packages\zeroc.ice.net\3.7.0\tools\netcoreapp2.0\iceboxnet.dll --Ice.Config=config.icebox
```

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |
