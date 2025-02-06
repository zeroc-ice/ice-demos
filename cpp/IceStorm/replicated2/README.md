# Replicated 2

This demo demonstrates how to configure and deploy [replicated IceStorm
instances][1] without using IceGrid.

To build the demo run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

To run the demo, start the three IceStorm replicas. In three separate
windows:

```shell
icebox --Ice.Config=config.ib1
icebox --Ice.Config=config.ib2
icebox --Ice.Config=config.ib3
```

And then in a separate window:

**Linux/macOS:**

```shell
./build/subscriber
```

**Windows:**

```shell
build\Release\subscriber
```

In another window:

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

With three replicas, any single replica can become unavailable, and
IceStorm will continue to function. However, should two replicas
become unavailable, replication will halt until another instance comes
back online.

Other than configuration, the subscribers and publishers do not have
to do anything special to take advantage of the replication. The
subscriber and publisher are identical to those contained in the clock
demo. See ../clock/README for instructions on running them.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icestorm/highly-available-icestorm
