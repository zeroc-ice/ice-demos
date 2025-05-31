# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plugin with replicated servers.

You can build the client and server applications with:

``` shell
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

## How it works

Each server instance creates a unique identifier and registers itself as part of the "greeterPool" replica group. When a client makes a request to the well-known object "greeter", the IceDiscovery plugin automatically routes the request to one of the available servers in the replica group.

You can observe which server handles each request by looking at the greeting message, which includes the server's unique identifier.
