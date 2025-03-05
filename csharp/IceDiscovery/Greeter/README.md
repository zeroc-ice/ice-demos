# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plugin. The IceDiscovery plugin allows a client application
to discover Ice objects without hardcoding any addressing information.

You can build the client and server applications with:

``` shell
dotnet build
```

Then, start the Server program:

```shell
cd Server
dotnet run --Ice.Trace.Locator
```

Finally, a separate terminal, start the Client program:

```shell
cd Client
dotnet run --Ice.Trace.Locator
```

>[!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plugin.
