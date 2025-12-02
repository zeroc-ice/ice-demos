# DataStorm Node

This demo illustrates how to use a DataStorm node to discover writers and readers without relying on UDP multicast.

It also shows how readers and writers can exchange data through the node even when their own server endpoints are
disabled.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.
- Install the DataStorm node (dsnode). See [Ice service installation].

## Building the demo

To build the demo, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

## Running the demo

To run the demo, start a DataStorm node:

```shell
dsnode --Ice.Config=node.conf
```

In a separate window, start the writer:

**Linux/macOS:**

```shell
./build/writer
```

**Windows:**

```shell
build\writer
```

In a separate window, start the reader:

**Linux/macOS:**

```shell
./build/reader
```

**Windows:**

```shell
build\reader
```

You can start multiple readers and writers.
The readers print the timestamps sent by the writers.

Stopping the DataStorm node:

- prevents communication between existing readers and writers, and
- stops discovery of new readers and writers.

Restarting the node resumes both data exchange and discovery.

## Enabling direct connections

You can enable a reader or writer’s server endpoints by passing:

```shell
--DataStorm.Node.Server.Enabled=1
```

When server endpoints are enabled (for either the reader or the writer), a direct connection is established between
them during discovery. The connection is made from the peer without server endpoints to the one with server endpoints.

Once a direct connection is established, stopping the DataStorm node no longer affects data exchange between these
peers—it only prevents the discovery of new readers and writers.

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
