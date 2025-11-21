# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

A Slice-defined exception should be seen as a custom error carried by the response instead of the expected return
value--there is naturally no throwing across the network.

## Prerequisites

- Install uv. See [Installing uv].

## Running the server

Navigate to the `server` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```shell
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the server

```shell
uv run main.py
```

## Running the client

In a separate terminal, navigate to the `client` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```shell
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the client

```shell
uv run main.py
```

[Installing uv]: https://docs.astral.sh/uv/getting-started/installation/
