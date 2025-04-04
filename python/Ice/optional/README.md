# Ice Optional

This demo shows how to use the `optional` keyword to change Slice definitions without breaking "on-the-wire"
interoperability.

The application is very simple: a weather station (hosted by our server) receives readings from sensors (the clients).

In the first version of this application and its Slice definitions, the atmospheric conditions we report include only
temperature and humidity. In the second version, we add a third reading, the pressure, as an optional field:

```ice
class AtmosphericConditions
{
    /// The temperature in degrees Celsius.
    double temperature;

    /// The humidity in percent.
    double humidity;

    /// The pressure in millibars (new in version 2 of the Slice definitions).
    optional(1) double pressure;
}
```

We recommend running each program in a separate Python virtual environment. If you are new to Python virtual environments,
see [Python Virtual Environments].

## Running the server

Navigate to the `server1` or `server2` directory, depending on whether you want to run version 1 or version 2 of the
server.

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

- Compile the appropriate Slice file using the Slice compiler for Python:

    For server1 use:

    ```shell
    slice2py ../slice/WeatherStation1.ice
    ```

    For server2 use:

    ```shell
    slice2py ../slice/WeatherStation2.ice
    ```

- Run the server program:

    ```shell
    python main.py
    ```

## Running the client

In a separate terminal, navigate to the `client1` or `client2` directory depending on whether you want to run version 1
or version 2 of the client.

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

- Compile the appropriate Slice file using the Slice compiler for Python:

    For client1 use:

    ```shell
    slice2py ../slice/WeatherStation1.ice
    ```

    For client2 use:

    ```shell
    slice2py ../slice/WeatherStation2.ice
    ```

- Run the client program:

    ```shell
    python main.py
    ```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly.

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
