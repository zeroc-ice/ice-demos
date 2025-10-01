# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plugin with replicated servers.

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First, start two or more Server programs, each in its own terminal:

```shell
swift run Server --Ice.Trace.Locator
```

Then, start the Client program:

```shell
swift run Client --Ice.Trace.Locator
```

> [!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plugin.
