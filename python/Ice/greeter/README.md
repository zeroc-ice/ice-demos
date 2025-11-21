# Ice Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server: a synchronous dispatch implementation (in `server`), and an
asynchronous dispatch implementation (in `server_amd`). The same client works with both.

## Prerequisites

- Install uv. See [Installing uv].

## Running the server

Navigate to the `server` or `server_amd` directory, depending on whether you want to run the synchronous or
asynchronous version of the server.

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
uv run main.py
```

[Installing uv]: https://docs.astral.sh/uv/getting-started/installation/
