This demo shows how to create an [IceBox][1] service.

To run the demo, first start the IceBox server:

| .NET Framework 4.5                     | .NET Core                                         |
| -------------------------------------- | ------------------------------------------------- |
| `iceboxnet --Ice.Config=config.icebox` | `dotnet iceboxnet.dll --Ice.Config=config.icebox` |

In a separate window, start the client:
```
client
```

> With .NET Core 2.x, use instead:
> ```
> dotnet client.dll
> ```

The demo build system copies the `iceboxnet` app to the demo's folder. With the
`zeroc.ice.net` NuGet package, this app is installed in a subfolder of `tools`:

| Platform            | Path                               |
| --------------------| ---------------------------------- |
| .NET Framework 4.5  | `tools\net45\iceboxnet.exe`        |
| .NET Core 2.1/2.2   | `tools\netcoreapp2.1\iceboxnet.dll`|
| .NET Core 3.0       | `tools\netcoreapp3.0\iceboxnet.dll`|

[1]: https://doc.zeroc.com/ice/4.0/icebox
