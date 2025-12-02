# IceGrid IceBox

The IceGrid IceBox demo illustrates how to deploy an IceBox server with IceGrid.

## Ice prerequisites

- Install IceGrid. See [Ice service installation].

## Building the demo

The demo consists of three Gradle projects:

- **client** — the client application (uses the Gradle [application plugin]).
- **service** — the Greeter service implementation (uses the [java-library plugin]).
- **iceboxserver** — a launcher for the IceBox server (uses the Gradle [application plugin]).

> The `iceboxserver` project is a small Java application that starts the IceBox server (`com.zeroc.IceBox.Server`).
> At runtime, its classpath includes the Ice and IceBox libraries as well as the Greeter service JAR.

To build the demo, run:

```shell
./gradlew installDist
```

This creates a self-contained distribution under build/install/ for each application.
Each distribution includes:

- a bin/ directory with start scripts, and
- a lib/ directory containing the application JAR and all its runtime dependencies.

In our IceGrid deployment, we call java directly and set the class path to include everything in the distribution’s lib/
directory. This ensures both the server JAR and its dependencies (such as Ice) are available at runtime.

## Running the demo

First, start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=registry.conf
```

Then, start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=node.conf
```

Next, deploy the "GreeterHall" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=admin.conf -e "application add greeter-hall.xml"
```

Finally, run the client application:

```shell
./gradlew :client:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
