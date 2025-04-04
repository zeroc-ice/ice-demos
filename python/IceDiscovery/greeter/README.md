# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plugin. The IceDiscovery plugin allows a client application
to discover Ice objects without hardcoding any addressing information.

We recommend running each program in a separate Python virtual environment. If you are new to Python virtual environments,
see [Python Virtual Environments].

## Running the server

Navigate to the `server` or `server_asyncio` directory, depending on whether you want to run the synchronous or
asynchronous server.

- Create and activate a Python virtual environment:

    ```shell
    python -m venv venv
    source venv/bin/activate  # On macOS/Linux
    venv\Scripts\activate     # On Windows
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
    python main.py --Ice.Trace.Locator
    ```

## Running the client

In a separate terminal, navigate to the `client` directory to run the client program.

- Create and activate a Python virtual environment:

    ```shell
    python -m venv venv
    source venv/bin/activate  # On macOS/Linux
    venv\Scripts\activate     # On Windows
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
    python main.py --Ice.Trace.Locator
    ```

>[!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plugin.

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
