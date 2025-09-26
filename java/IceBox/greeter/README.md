# IceBox Greeter

This demo shows how to create an IceBox service in Java.

## Building the demo

The demo consists of three Gradle projects:

- **client** — the client application (uses the Gradle [application plugin]).
- **service** — the Greeter service implementation (uses the [java-library plugin]).
- **iceboxserver** — a launcher for the IceBox server (uses the Gradle [application plugin]).

> The `iceboxserver` project is a small Java application that starts the IceBox server (`com.zeroc.IceBox.Server`).
> At runtime, its classpath includes the Ice/IceBox libraries as well as the Greeter service JAR.

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

First, start the IceBox server:

```shell
./gradlew :iceboxserver:run --quiet --args="--IceBox.Service.Greeter=com.example.icebox.greeter.service.GreeterService --Ice.Trace.Dispatch"
```

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet
```

[application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
[java-library plugin]: https://docs.gradle.org/current/userguide/java_library_plugin.html
