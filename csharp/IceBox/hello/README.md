To run this demo, open two Command Prompt windows.

In the first window, start the IceBox server:

To run the demo, first start the IceBox server:

| .NET Framework 4.5                     | .NET Core 2.0                                    |
| -------------------------------------- | ------------------------------------------------ |
| `iceboxnet --Ice.Config=config.icebox` | `dotnet iceboxnet.dll --Ice.Config=config.icebox`|

The demo build system copies the `iceboxnet` app to the demo's folder. With the `zeroc.ice.net`
NuGet package, this app is installed in a subfolder of `tools`:

| .NET Framework 4.5          | .NET Core 2.0                      |
| --------------------------- | ---------------------------------- |
| `tools\net45\iceboxnet.exe` | `tools\netcoreapp2.0\iceboxnet.dll`|

In the second window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |
