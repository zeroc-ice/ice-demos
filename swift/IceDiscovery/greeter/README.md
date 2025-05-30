# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plugin. The IceDiscovery plugin allows a client application
to discover Ice objects without hardcoding any addressing information.

You can build the client and server applications with:

``` shell
swift build
```

Then, start the Server program:

```shell
swift run Server --Ice.Trace.Locator
```

Finally, a separate terminal, start the Client program:

```shell
swift run Client --Ice.Trace.Locator
```

> [!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plugin.
