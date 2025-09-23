# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plug-in with replicated servers.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.

## Building the demo

To build the client and server applications, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

## Running the demo

Then, start two or more server programs, each in its own terminal:

**Linux/macOS:**

```shell
./build/server --Ice.Trace.Locator
```

**Windows:**

```shell
build\server --Ice.Trace.Locator
```

Finally, start the client program:
**Linux/macOS:**

```shell
./build/client --Ice.Trace.Locator
```

**Windows:**

```shell
build\client --Ice.Trace.Locator
```

>[!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plug-in.

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
