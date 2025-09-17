# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plug-in with replicated servers.

To build the client and server applications, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

Then, start two ore more server programs, each in its own terminal:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\server
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
