# IceStorm Clock

## Overview

This demo shows how to use [IceStorm][1].

To run the demo, start the IceStorm service:

```shell
icebox --Ice.Config=config.icebox
```

This configuration assumes there is a subdirectory named db in the current working directory.

In a separate window:

```shell
swift run Subscriber
```

In another window:

```shell
swift run Publisher
```

While the publisher continues to run, "tick" messages should be displayed in the subscriber window.

## Options

Both the subscriber and publisher take an optional topic name as a final argument. The default value for this topic
is "time".

Through the use of command-line options, both the subscriber and publisher can use different QoS for sending and
receiving messages.

## Subscriber Options

```shell
swift run Subscriber --oneway
```

The subscriber receives events as oneway messages. This is the default.

```shell
swift run Subscriber --datagram
```

The subscriber receives events as datagrams.

```shell
swift run Subscriber --twoway
```

The subscriber receives events as twoway messages.

```shell
swift run Subscriber --ordered
```

The subscriber receives events as twoway messages with guaranteed ordering.

```shell
swift run Subscriber --batch
```

This is an additional flag that forwards datagram and oneway events to the subscriber in batches.

```shell
swift run Subscriber --id <id>
```

This option specifies a unique identity for this subscriber. When you use this option, you should also run the
subscriber on a fixed port by setting the `Clock.Subscriber.Endpoints` property. For example:

```shell
swift run Subscriber --Clock.Subscriber.Endpoints="tcp -p <port>"
```

Replace "tcp" with "udp" when using the --datagram option.

```shell
swift run Subscriber --retryCount <count>
```

This option sets the retry count for a subscriber. This option should be used in conjunction with the --id option.
Setting retryCount changes the default subscriber QoS to twoway.

## Publisher Options

```shell
swift run Publisher --oneway
```

The publisher sends events as oneway messages. This is the default.

```shell
swift run Publisher --datagram
```

The publisher sends events as datagrams.

```shell
swift run Publisher --twoway
```

The publisher sends events as twoway messages.

## Running the demo on several hosts

You must modify several configuration files in order to run the publisher, subscriber, and IceStorm service on
separate hosts. Let us assume that IceStorm is running on host `<I>`. Replace `<I>` with the appropriate hostname or
IP address in the steps below:

1. Edit the subscriber's configuration file, `config.sub`, and change the `TopicManager.Proxy` property to

   ```text
   TopicManager.Proxy=DemoIceStorm/TopicManager:tcp -h <I> -p 10000
   ```

2. Edit the publisher's configuration file, `config.pub`, and change the `TopicManager.Proxy` property to

   ```text
   TopicManager.Proxy=DemoIceStorm/TopicManager:tcp -h <I> -p 10000
   ```

If you experience any network delays or errors, edit all of the configuration files and enable the `Ice.Trace.Network`
property. Running the processes with this property enabled displays a log of Ice's network activity and may help you to
identify the problem more quickly.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icestorm
