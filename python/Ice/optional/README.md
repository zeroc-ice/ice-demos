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

We recommend using [uv] for building and running the demos.

## Running the server

Navigate to the `server1` or `server2` directory, depending on whether you want to run version 1 or version 2 of the server.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the appropriate Slice file:

- For **server1**:

  ```bash
  uv run slice2py ../slice/WeatherStation1.ice
  ```

- For **server2**:

  ```bash
  uv run slice2py ../slice/WeatherStation2.ice
  ```

### 2. Run the server

```bash
uv run main.py
```

## Running the client

In a separate terminal, navigate to the `client1` or `client2` directory, depending on whether you want to run version
1 or version 2 of the client.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the appropriate Slice file:

- For **client1**:

  ```bash
  uv run slice2py ../slice/WeatherStation1.ice
  ```

- For **client2**:

  ```bash
  uv run slice2py ../slice/WeatherStation2.ice
  ```

### 2. Run the client

```bash
uv run main.py
```

Thanks to the `optional` keyword, version 1 and version 2 of the clients and servers interoperate seamlessly.

[uv]: https://docs.astral.sh/uv/
