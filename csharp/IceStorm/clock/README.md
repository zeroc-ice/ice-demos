# Overview

To run the demo, start the IceStorm service:
```
icebox --Ice.Config=config.icebox
```

This configuration assumes there is a subdirectory named `db` in the
current working directory.

In a separate window:

| .NET Framework 4.5 | .NET Core 2.0           |
| ------------------ | ----------------------- |
| `subscriber`       | `dotnet subscriber.dll` |

In another window:

| .NET Framework 4.5 | .NET Core 2.0          |
| ------------------ | ---------------------- |
| `publisher`        | `dotnet publisher.dll` |

While the publisher continues to run, "tick" messages should be
displayed in the subscriber window.

# Options

Both the subscriber and publisher take an optional topic name as a
final argument. The default value for this topic is `time`.

Through the use of command-line options both the subscriber and
publisher can use different QoS for sending and receiving messages.

## For the subscriber

| .NET Framework 4.5    | .NET Core 2.0                    |
| --------------------- | -------------------------------- |
| `subscriber --oneway` | `dotnet subscriber.dll --oneway` |

The subscriber receives events as oneway messages. This is the
default.

| .NET Framework 4.5      | .NET Core 2.0                      |
| ----------------------- | ---------------------------------- |
| `subscriber --datagram` | `dotnet subscriber.dll --datagram` |

The subscriber receives events as datagrams.

| .NET Framework 4.5    | .NET Core 2.0                    |
| --------------------- | -------------------------------- |
| `subscriber --twoway` | `dotnet subscriber.dll --twoway` |

The subscriber receives events as twoway messages.

| .NET Framework 4.5     | .NET Core 2.0                     |
| ---------------------- | --------------------------------- |
| `subscriber --ordered` | `dotnet subscriber.dll --ordered` |

The subscriber receives events as twoway messages with guaranteed
ordering.

| .NET Framework 4.5   | .NET Core 2.0                   |
| -------------------- | ------------------------------- |
| `subscriber --batch` | `dotnet subscriber.dll --batch` |

This is an additional flag that forwards datagram and oneway events
to the subscriber in batches.

| .NET Framework 4.5     | .NET Core 2.0                     |
| ---------------------- | --------------------------------- |
| `subscriber --id <id>` | `dotnet subscriber.dll --id <id>` |

This option specifies a unique identity for this subscriber. When
you use this option, you should also run the subscriber on a fixed
port by setting the `Clock.Subscriber.Endpoints` property. For
example:

| .NET Framework 4.5                                                  | .NET Core 2.0                                                                  |
| ------------------------------------------------------------------- | ------------------------------------------------------------------------------ |
| `subscriber --Clock.Subscriber.Endpoints="tcp -p <port> -h <host>"` | `dotnet subscriber.dll --Clock.Subscriber.Endpoints="tcp -p <port> -h <host>"` |

Replace `tcp` with `udp` when using the `--datagram` option.

| .NET Framework 4.5                | .NET Core 2.0                                |
| --------------------------------- | -------------------------------------------- |
| `subscriber --retryCount <count>` | `dotnet subscriber.dll --retryCount <count>` |

This option sets the retry count for a subscriber. This option
should be used in conjunction with the `--id` option. Setting
`retryCount` changes the default subscriber QoS to twoway.

## For the publisher

| .NET Framework 4.5   | .NET Core 2.0                   |
| -------------------- | ------------------------------- |
| `publisher --oneway` | `dotnet publisher.dll --oneway` |

The publisher sends events as oneway messages. This is the default.

| .NET Framework 4.5     | .NET Core 2.0                     |
| ---------------------- | --------------------------------- |
| `publisher --datagram` | `dotnet publisher.dll --datagram` |

The publisher sends events as datagrams.

| .NET Framework 4.5   | .NET Core 2.0                   |
| -------------------- | ------------------------------- |
| `publisher --twoway` | `dotnet publisher.dll --twoway` |

The publisher sends events as twoway messages.

## Running the demo on several hosts

You must modify several configuration files in order to run the
publisher, subscriber, and IceStorm service on separate hosts. For
the sake of discussion, let us assume that the processes are running
on the following hosts:
```
Host <P>: publisher
Host <S>: subscriber
Host <I>: IceStorm
```
Replace `<P>`, `<S>`, and `<I>` with the appropriate host names or IP addresses
in the steps below:

1. Edit the subscriber's configuration file, `config.sub`, and change
   the `TopicManager.Proxy` property to
   ```
   TopicManager.Proxy=DemoIceStorm/TopicManager:default -h <I> -p 10000
   ```
2. Edit the publisher's configuration file, `config.pub`, and change
   the `TopicManager.Proxy` property to
   ```
   TopicManager.Proxy=DemoIceStorm/TopicManager:default -h <I> -p 10000
   ```
3. Since the default configurations for the subscriber and IceStorm
   endpoints listen only on localhost, you must also modify the Endpoints
   properties to explicitly include the host machine's name or IP
   address.

   In `config.sub`:
   ```
   Clock.Subscriber.Endpoints=tcp -h <S>:udp -h <S>
   ```
   In `config.service`:
   ```
   IceStorm.TopicManager.Endpoints=default -h <I> -p 10000
   IceStorm.Publish.Endpoints=tcp -h <I> -p 10001:udp -h <I> -p 10001
   ```

If you experience any network delays or errors, edit all of the
configuration files and enable the `Ice.Trace.Network` property. Running
the processes with this property enabled displays a log of Ice's
network activity and may help you to identify the problem more
quickly.
