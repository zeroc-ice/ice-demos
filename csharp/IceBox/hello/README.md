To run this demo, open two Command Prompt windows.

In the first window, start the IceBox server for .NET. The IceBox server for .NET
(`iceboxnet.exe`) is in the `tools` folder of your  `zeroc.ice.net` NuGet. You should
add this folder to your PATH with a command similar to:
```
set PATH=%USERPROFILE%\ice-demos\csharp\packages\zeroc.ice.net.3.7.0\tools;%PATH%
```

Then start IceBox:
```
iceboxnet --Ice.Config=config.icebox
```

In the second window, run the client:
```
client
```
