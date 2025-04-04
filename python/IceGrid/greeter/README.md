# IceGrid Greeter

The IceGrid Greeter demo illustrates how to create a very simple IceGrid deployment that manages a Greeter server.

Follow these steps to build and run the demo:

1 Navigate to the server directory and create a Python virtual environment for running the server.

    ```shell
    python -m venv venv
    source venv/bin/activate  # On macOS/Linux
    venv\Scripts\activate     # On Windows
    ```
2. Install the server program dependencies:

    ```shell
    pip install -r requirements.txt
    ```

3. Compile the Greeter.ice file with the Slice compiler for Python:

    ```shell
    slice2py ../slice/Greeter.ice
    ```

4. Start the IceGrid registry in its own terminal:

    ```shell
    icegridregistry --Ice.Config=config.registry
    ```

5. Start the IceGrid node in its own terminal:

    ```shell
    source server/venv/bin/activate  # On macOS/Linux
    server\venv\Scripts\activate     # On Windows
    icegridnode --Ice.Config=config.node
    ```

    We need to start icegridnode from an active Python virtual environment in order for it to find the server
    program dependencies installed in the virtual environment.

6. Deploy the "GreeterHall" application in this IceGrid deployment:

    ```shell
    icegridadmin --Ice.Config=config.admin -e "application add greeter-hall.xml"
    ```

    `greeter-hall.xml` configures a single Greeter server. As an alternative, you can deploy 3 replicated Greeter
    servers with:

    ```shell
    icegridadmin --Ice.Config=config.admin -e "application add greeter-hall-with-replication.xml"
    ```

7. Using a separate terminal create a Python virtual environment for running the client.
    ```shell
    python -m venv venv
    source venv/bin/activate  # On macOS/Linux
    venv\Scripts\activate     # On Windows
    ```

8. Install the client program dependencies:

    ```shell
    pip install -r requirements.txt
    ```

9. Compile the Greeter.ice file with the Slice compiler for Python:

    ```shell
    slice2py ../slice/Greeter.ice
    ```

10. Run the client program:

    ```shell
    python main.py
    ```
