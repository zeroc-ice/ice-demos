To run this demo, open two terminal windows. In the first window,
start the IceBox server:

```
      icebox++11 --Ice.Config=config.icebox
```

The name of the IceBox executable may be different for your plaform,
for example:

      `icebox++11d` for a Debug IceBox server on Windows
      `icebox32++11` for a 32-bit IceBox server on a x86_64 Linux host

You can find more details in the Ice Manual:
https://doc.zeroc.com/display/Ice37/Starting+the+IceBox+Server

In the second window, run the client:
```
      client
```

To shut down IceBox, use `iceboxadmin`:
```
      iceboxadmin --Ice.Config=config.admin shutdown
```