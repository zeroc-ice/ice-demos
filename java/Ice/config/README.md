# Ice Config

This demo shows how to configure client and server applications using Ice configuration files. The configuration we
demonstrate sets up SSL: the properties in `client.conf` and `server.conf` select `ssl` as the default transport
protocol and specify the certificates used by IceSSL; `server.conf` also turns on tracing.

> [!NOTE]
> On Windows, run all the commands below in Git Bash or PowerShell; they don't work in the cmd.exe Command Prompt.

## Building the demo

The demo has two Gradle projects, **client** and **server**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew installDist
```

This creates a self-contained distribution under build/install/ for each application, with launcher scripts in its
bin/ directory.

## Running the demo

First, start the server application from this demo's directory:

```shell
./server/build/install/server/bin/server
```

Then, in a separate terminal, start the client application from this demo's directory:

```shell
./client/build/install/client/bin/client
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
