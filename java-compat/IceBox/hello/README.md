This demo shows how to create an [IceBox][1] service.

To run this demo, open two terminal windows. In the first window,
start the IceBox server:

```
java -jar build/libs/iceboxserver.jar --Ice.Config=config.icebox
```

In the second window, run the client:

```
java -jar build/libs/client.jar
```

To shut down IceBox, use IceBox.Admin:

```
java IceBox.Admin --Ice.Config=config.admin shutdown
```

[1]: https://doc.zeroc.com/ice/3.7/icebox
