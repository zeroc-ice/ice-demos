# IceDiscovery Replication

This demo illustrates how to use the IceDiscovery plug-in with replicated servers.

## Prerequisites

- Install uv. See [Installing uv].

## Running the server

Navigate to the `server` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```bash
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the server

First, start two or more Server programs, each in its own terminal:

```bash
uv run main.py --Ice.Trace.Locator
```

## Running the client

In a separate terminal, navigate to the `client` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```bash
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the client

```bash
uv run main.py --Ice.Trace.Locator
```

> [!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it enables tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plug-in.

[Installing uv]: https://docs.astral.sh/uv/getting-started/installation/
