This demo shows how to create an [IceBox][1] service.

To run the demo, first start the IceBox server:

| .NET Framework 4.5                     | .NET Core 3.0                                    |
| -------------------------------------- | ------------------------------------------------ |
| `iceboxnet --Ice.Config=config.icebox` | `dotnet iceboxnet.dll --Ice.Config=config.icebox`|

The demo build system copies the `iceboxnet` app to the demo's folder. With the `zeroc.ice.net`
NuGet package, this app is installed in a subfolder of `tools`:

| .NET Framework 4.5          | .NET Core 3.0                      |
| --------------------------- | ---------------------------------- |
| `tools\net45\iceboxnet.exe` | `tools\netcoreapp3.0\iceboxnet.dll`|

In the second window, start the client:

| .NET Framework 4.5 | .NET Core 3.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

[1]: https://doc.zeroc.com/ice/3.7/icebox
