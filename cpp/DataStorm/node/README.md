This demo illustrates the use of a DataStorm node to discover writers and readers without using UDP multicast.

It also demonstrates how readers and writers can exchange data through the DataStorm node when the server endpoints
of both the writers and readers are disabled.

To run the demo, start a DataStorm node:

```shell
dsnode --Ice.Config=config.node
```

In a separate window, start the writer:

```shell
writer
```

In a separate window, start the reader:

```shell
reader
```

You can start multiple readers and writers. The readers print the time sent by the writers. Stopping the DataStorm node
prevents communications between readers and writers and the discovery of new readers and writers. Restarting the node
resumes data exchange and discovery.

You can enable the reader or the writer's server endpoints by editing the `config.writer` or `config.reader` file. When
server endpoints are enabled (either for the reader or the writer), a direct connection is established between the
reader and the writer during the discovery process. The connection is from the reader or writer without server endpoints
to the reader or writer with server endpoints. Stopping the DataStorm node at this point does not impede communications
between readers and writers (since they are directly connected to one another); it does however prevent discovery of new
readers and writers.
