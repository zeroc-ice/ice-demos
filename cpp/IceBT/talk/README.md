# IceBT Talk

This demo shows how to use [IceBT][1], the Bluetooth transport plug-in.
The command-line program allows you to hold a peer-to-peer talk session
with another device on Linux or Android.

To build the demo, run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

To run the demo, just start the program:

```shell
talk
```

On another device, start the command-line program or Android app.

[1]: https://doc.zeroc.com/display/Ice37/IceBT
