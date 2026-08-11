# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

This demo provides a client application that works with the IceGrid/greeter demo and reuses its server components and
IceGrid configuration.

> [!NOTE]
> On Windows, run all the commands below in Git Bash or PowerShell; not all of them work in the cmd.exe Command
> Prompt.

## Ice prerequisites

- Install IceGrid. See [Ice service installation].

## Building the demo

The demo has a single Gradle project **client** that uses the [application plugin].

To build the demo, run:

```shell
./gradlew installDist
```

This creates a self-contained distribution under build/install/ for the client application, with launcher scripts in
its bin/ directory.

## Running the demo

First, build the [IceGrid/greeter](../greeter) demo.

Then, run `icegridregistry`, `icegridnode`, and configure IceGrid using `icegridadmin` as per the IceGrid Greeter demo
instructions.

Then, in a separate terminal, start the client application:

```shell
./client/build/install/client/bin/client
```

[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
