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

> [!NOTE]
> On Windows, run all the commands below in Git Bash or PowerShell; they don't work in the cmd.exe Command Prompt.

## Building the demo

The demo has four Gradle projects, all using the [application plugin].

- **client1** A client application using version 1 of the Slice definitions.
- **client2** A client application using version 2 of the Slice definitions.
- **server1** A server application using version 1 of the Slice definitions.
- **server2** A server application using version 2 of the Slice definitions.

To build the demo, run:

```shell
./gradlew installDist
```

This creates a self-contained distribution under build/install/ for each application, with launcher scripts in its
bin/ directory.

## Running the demo

First, start either version 1 or version 2 of the server in its own terminal:

```shell
./server1/build/install/server1/bin/server1
```

or

```shell
./server2/build/install/server2/bin/server2
```

Then, in a separate terminal, run version 1 and then version 2 of the Client:

```shell
./client1/build/install/client1/bin/client1
```

and

```shell
./client2/build/install/client2/bin/client2
```

Thanks to `optional`, version 1 and version 2 of the clients and servers interoperate seamlessly:

|               | Server v1                                            | Server v2                          |
|---------------|------------------------------------------------------|------------------------------------|
| **Client v1** | The reading has no pressure field.                   | The reading's pressure is not set. |
| **Client v2** | The server ignores the pressure sent by the client.  | The reading includes the pressure. |

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
