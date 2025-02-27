# Replicated

This demo demonstrates how to configure and deploy [Highly Available
IceStorm][1] using IceGrid.

To build the demo run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

To run the demo, start the IceGrid service:

```shell
icegridnode --Ice.Config=config.grid
```

In a separate window:

```shell
icegridadmin --Ice.Config=config.grid -e "application add application.xml"
```

Next run the subscriber:

**Linux/macOS:**

```shell
./build/subscriber
```

**Windows:**

```shell
build\Release\subscriber
```

In a separate window, start the publisher:

**Linux/macOS:**

```shell
./build/publisher
```

**Windows:**

```shell
build\Release\publisher
```

While the publisher continues to run, "tick" messages should be
displayed in the subscriber window.

The IceGrid application described in `application.xml' deploys three
IceStorm service replicas. With three replicas, any single replica can
become unavailable, and IceStorm will continue to function. However,
should two replicas become unavailable, replication will halt until another
instance comes back online.

Other than configuration, the subscribers and publishers do not have
to do anything special to take advantage of the replication. The
subscriber and publisher are identical to those contained in the clock
demo. See ../clock/README for instructions on running them.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icestorm/highly-available-icestorm
