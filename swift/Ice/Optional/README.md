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

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

Start either version 1 or version 2 of the server in its own terminal:

```shell
swift run Server1
```

or

```shell
swift run Server2
```

In a separate terminal, run version 1 and then version 2 of the Client:

```shell
swift run Client1
```

and

```shell
swift run Client2
```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly.
