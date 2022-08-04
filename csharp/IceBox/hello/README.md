This demo shows how to create an [IceBox][1] service.

To run the demo, first start the IceBox server:

| .NET Framework 4.5                     | .NET 6.0                                          |
| -------------------------------------- | ------------------------------------------------- |
| `iceboxnet --Ice.Config=config.icebox` | `dotnet iceboxnet.dll --Ice.Config=config.icebox` |

In a separate window, start the client:
```
client
```

> With .NET 6, use instead:
> ```
> dotnet client.dll
> ```

The demo build system copies the `iceboxnet` app to the demo's folder. With the
`zeroc.ice.net` NuGet package, this app is installed in a subfolder of `tools`:

| Platform            | Path                               |
| --------------------| ---------------------------------- |
| .NET 6.0            | `tools\net6.0\iceboxnet.dll`       |
| .NET Framework 4.5  | `tools\net45\iceboxnet.exe`        |

[1]: https://doc.zeroc.com/ice/3.7/icebox
