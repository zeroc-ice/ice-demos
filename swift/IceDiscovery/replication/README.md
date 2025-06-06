# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plugin with replicated servers.

You can build the client and server applications with:

```shell
swift build
```

Then, start two or more Server programs, each in its own terminal:

```shell
swift run Server --Ice.Trace.Locator
```

Finally, start the Client program:

```shell
swift run Client --Ice.Trace.Locator
```

> [!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plugin.
