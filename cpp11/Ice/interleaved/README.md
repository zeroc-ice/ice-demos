The interleaved demo is a variant of the throughput demo that uses asynchronous
requests to send "echo" requests to the server. This way, the client sends
multiple concurrent requests and we maximize the bandwidth usage.

To run the demo, first start the server:

```
server
```

In a separate window, start the client:

```
client
```

See the throughput demo for a description of the metadata used in
Throughput.ice.
