# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plug-in with replicated servers.

You can build the client and server applications with:

``` shell
dotnet build
```

Then, start two or more Server programs, each in its own terminal:

```shell
cd Server
dotnet run --no-build --Ice.Trace.Locator
```

Finally, start the Client program:

```shell
cd Client
dotnet run --Ice.Trace.Locator
```

>[!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plug-in.
