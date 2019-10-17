Overview
========

This demo shows how to use [IceStorm][1].

To run the demo, start the IceStorm service:

```
icebox --Ice.Config=config.icebox
```

This configuration assumes there is a subdirectory named db in the
current working directory.

In a separate window:

```
python subscriber.py
```

In another window:

```
python publisher.py
```

While the publisher continues to run, "tick" messages should be
displayed in the subscriber window.

Options
=======

Both the subscriber and publisher take an optional topic name as a
final argument. The default value for this topic is "time".

Through the use of command-line options both the subscriber and
publisher can use different QoS for sending and receiving messages.

Subscriber Options
------------------

```
python subscriber.py --oneway
```

The subscriber receives events as oneway messages. This is the
default.

```
python subscriber.py --datagram
```

The subscriber receives events as datagrams.

```
python subscriber.py --twoway
```

The subscriber receives events as twoway messages.

```
python subscriber.py --ordered
```

The subscriber receives events as twoway messages with guaranteed
ordering.

```
python subscriber.py --batch
```

This is an additional flag that forwards datagram and oneway events
to the subscriber in batches.

```
python subscriber.py --id <id>
```

This option specifies a unique identity for this subscriber. When
you use this option, you should also run the subscriber on a fixed
port by setting the `Clock.Subscriber.Endpoints` property. For
example:

```
python subscriber.py --Clock.Subscriber.Endpoints="tcp -p <port> -h <host>"
```

Replace "tcp" with "udp" when using the --datagram option.

```
python Subscriber.py --retryCount <count>
```

This option sets the retry count for a subscriber. This option
should be used in conjunction with the --id option. Setting
retryCount changes the default subscriber QoS to twoway.

Publisher Options
-----------------

```
python publisher.py --oneway
```

The publisher sends events as oneway messages. This is the default.

```
python Publisher.py --datagram
```

The publisher sends events as datagrams.

```
python Publisher.py --twoway
```

The publisher sends events as twoway messages.

Running the demo on several hosts
=================================

You must modify several configuration files in order to run the
publisher, subscriber, and IceStorm service on separate hosts. For
the sake of discussion, let us assume that the processes are running
on the following hosts:

 - Host `<P>`: publisher
 - Host `<S>`: subscriber
 - Host `<I>`: IceStorm

Replace `<P>`, `<S>`, and `<I>` with the appropriate host names or IP
addresses in the steps below:

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

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icestorm
