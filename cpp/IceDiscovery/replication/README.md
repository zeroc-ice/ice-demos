# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plugin with replicated servers.

To build the client and server applications, run:

```shell
cmake -B build
cmake --build build --config Release
```

Then, start two ore more server programs, each in its own terminal:

**Linux/macOS:**

```shell
./build/client --Ice.Trace.Locator
```

**Windows:**

```shell
build\Release\client --Ice.Trace.Locator
```

Finally, start the client program:
**Linux/macOS:**

```shell
./build/client --Ice.Trace.Locator
```

**Windows:**

```shell
build\Release\client --Ice.Trace.Locator
```

>[!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plugin.
