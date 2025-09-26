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

The demo has four Gradle projects, all using the [application plugin].

- **client1** A client application using version 1 of the Slice definitions.
- **client2** A client application using version 2 of the Slice definitions.
- **server1** A server application using version 1 of the Slice definitions.
- **server2** A server application using version 2 of the Slice definitions.

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

First, start either version 1 or version 2 of the server in its own terminal:

```shell
./gradlew :server1:run --quiet
```

or

```shell
./gradlew :server2:run --quiet
```

Then, in a separate terminal, run version 1 and then version 2 of the Client:

```shell
./gradlew :client1:run --quiet
```

and

```shell
./gradlew :client2:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
