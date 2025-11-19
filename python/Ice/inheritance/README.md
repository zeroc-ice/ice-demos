# Ice Inheritance

The Inheritance demo shows how to write a simple filesystem application using interface inheritance.

## Prerequisites

- [uv] Python package and project manager.

## Running the server

Navigate to the `server` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Filesystem.ice` file:

```bash
uv run slice2py ../slice/Filesystem.ice
```

### 2. Run the server

```bash
uv run main.py
```

## Running the client

In a separate terminal, navigate to the `client` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Filesystem.ice` file:

```bash
uv run slice2py ../slice/Filesystem.ice
```

### 2. Run the client

```bash
uv run main.py
```

[uv]: https://docs.astral.sh/uv/
