This demo shows how to create an [IceBox][1] service.

To run this demo, open two terminal windows. In the first window,
start the IceBox server:
```
icebox --Ice.Config=config.icebox
```

In the second window, run the client:
```
client
```

To shut down IceBox, use `iceboxadmin`:
```
iceboxadmin --Ice.Config=config.admin shutdown
```

Linux 32-bit
------------

If you are using 32-bit binaries on a Linux 64-bit host, use
`icebox32` instead of `icebox` to start the IceBox server.

[1]: https://doc.zeroc.com/ice/3.7/icebox
