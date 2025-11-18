# Ice Invocation Timeout

The Invocation Timeout demo shows how to use invocation timeouts.

We recommend using [uv] for building and running the demos.

## Running the server

Navigate to the `server` directory to run the server program.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```bash
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the server

```bash
uv run main.py
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
uv run main.py --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> `InvocationTimeoutException` does not close the connection.

[uv]: https://docs.astral.sh/uv/
