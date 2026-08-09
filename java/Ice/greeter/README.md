# Ice Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server:

- `server`: A synchronous dispatch implementation
- `serveramd`: An asynchronous dispatch implementation using Ice's Asynchronous Method Dispatch (AMD)

The same client works with both.

> [!NOTE]
> On Windows, run all the commands below in Git Bash or PowerShell; they don't work in the cmd.exe Command Prompt.

## Building the demo

The demo has three Gradle projects, **client**, **server**, and **serveramd**, all using the [application plugin].

To build the demo, run:

```shell
./gradlew installDist
```

This creates a self-contained distribution under build/install/ for each application, with launcher scripts in its
bin/ directory.

## Running the demo

First, start one of the server applications:

```shell
./server/build/install/server/bin/server
```

or

```shell
./serveramd/build/install/serveramd/bin/serveramd
```

Then, in a separate terminal, start the client application:

```shell
./client/build/install/client/bin/client
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
