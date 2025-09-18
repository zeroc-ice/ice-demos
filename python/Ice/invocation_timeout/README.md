# Ice Invocation Timeout

The Invocation Timeout demo shows how to use invocation timeouts.

We recommend running each program in a separate Python virtual environment. If you are new to Python virtual
environments, see [Python Virtual Environments].

## Running the server

Navigate to the `server` directory to run the server program.

- Create and activate a Python virtual environment:

    **On macOS and Linux**

    ```shell
    python3 -m venv venv
    source venv/bin/activate
    ```

    **On Windows**

    ```powershell
    python -m venv venv
    venv\Scripts\activate
    ```

- Install the program dependencies:

  ```shell
  pip install -r requirements.txt
  ```

- Compile the Greeter.ice file with the Slice compiler for Python:

  ```shell
  slice2py ../slice/Greeter.ice
  ```

- Run the server program:

  ```shell
  python main.py
  ```

## Running the client

In a separate terminal, navigate to the `client` directory to run the client program.

- Create and activate a Python virtual environment:

    **On macOS and Linux**

    ```shell
    python3 -m venv venv
    source venv/bin/activate
    ```

    **On Windows**

    ```powershell
    python -m venv venv
    venv\Scripts\activate
    ```

- Install the program dependencies:

```shell
pip install -r requirements.txt
```

- Compile the Greeter.ice file with the Slice compiler for Python:

```shell
slice2py ../slice/Greeter.ice
```

- Run the client program:

```shell
python main.py --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> `InvocationTimeoutException` does not close the connection.

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
