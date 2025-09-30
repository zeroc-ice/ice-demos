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

navigate to the `client1` or `client2` directory, depending on whether you want to build version 1 or version 2 of the
client.

First install npm dependencies:

```shell
npm install
```

Then, to build the client application, run:

```shell
npm run build
```

## Running the demo

First, start the server application.

Ice for JavaScript has limited server-side support. As a result, you need to start an Option server implemented in a
language that fully supports server-side functionality, such as C++, C#, Java, Python, or Swift.

You can start either version 1 or version 2 of the server.

Then navigate to the client1 directory and run version 1 of the client:

```shell
node client.js
```

Next, navigate to the client2 directory and run version 2 of the client:

```shell
node client.js
```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly.
