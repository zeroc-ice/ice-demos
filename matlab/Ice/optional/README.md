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

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

Ice for MATLAB supports only client-side applications. As a result, you first need to start an Ice Optional server
implemented in a language with server-side support, such as Python, Java, or C#.

You can start either version 1 or version 2 of the server.

Then, in the MATLAB console:

- Go to the Ice/optional/client1 or client2 directory

```shell
cd matlab/Ice/optional/client1
```

or

```shell
cd matlab/Ice/optional/client2
```

- Compile the WeatherStation.ice file with the Slice compiler for MATLAB

```shell
slice2matlab WeatherStation.ice
```

- Run the client application

```shell
client
```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly.

[Ice for MATLAB installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-matlab
