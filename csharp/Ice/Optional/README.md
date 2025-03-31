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
    ["cs:identifier:Temperature"]
    double temperature;

    /// The humidity in percent.
    ["cs:identifier:Humidity"]
    double humidity;

    /// The pressure in millibars (new in version 2 of the Slice definitions).
    ["cs:identifier:Pressure"]
    optional(1) double pressure;
}
```

You can build all the client and server applications with:

``` shell
dotnet build
```

Then, start either version 1 or version 2 of the server in its own terminal:

```shell
cd Server1
dotnet run
```

or

```shell
cd Server2
dotnet run
```

In a separate terminal, run version 1 and then version 2 of the Client:

```shell
cd Client1
dotnet run
```

and

```shell
cd Client2
dotnet run
```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly.
