# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plug-in. The IceDiscovery plug-in allows a client application
to discover Ice objects without hardcoding any addressing information.

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First, start the Server program:

```shell
swift run Server --Ice.Trace.Locator
```

Then, in a separate terminal, start the Client program:

```shell
swift run Client --Ice.Trace.Locator
```

> [!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plug-in.
