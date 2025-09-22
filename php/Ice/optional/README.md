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

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to start a Greeter server implemented
in a language with server-side support, such as Python, Java, or C#.

You can start either version 1 or version 2 of the server.

Then, in a separate window:

- Go to the Ice/optional/client1 or client2 directory

```shell
cd php/Ice/optional/client1
```

or

```shell
cd php/Ice/optional/client2
```

- Compile the WeatherStation.ice file with the Slice compiler for PHP

```shell
slice2php WeatherStation.ice
```

- Run the client application

```shell
php Client.php
```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly.

[Ice for PHP installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-php
