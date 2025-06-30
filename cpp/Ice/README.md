Demos in this directory:

- [bidir](./bidir)

  This demo shows how to use bidirectional connections for callbacks.
  This is typically used if the server cannot open a connection to the
  client to send callbacks, for example, because firewalls block
  incoming connections to the client.

- [callback](./callback)

  A simple callback demo that illustrates how a client can pass a
  proxy to a server, invoke an operation in the server, and the server
  call back into an object provided by the client as part of that
  invocation.

- [context](./context)

  This demo shows how to use Ice request contexts.

- [greeter](./greeter) TBD

- [interceptor](./interceptor)

  This demo shows how to use dispatch interceptors to secure operations
  with a token based authentication system, in addition to the use of
  implicit contexts.

- [interleaved](./interleaved)

  This demo is a variant of the throughput demo that uses asynchronous requests
  to send "echo" requests to the server.

- [invoke](./invoke)

  This demo illustrates the use of the Ice streaming API.

- [latency](./latency)

  A simple latency test that measures the basic call dispatch delay of
  Ice.

- [locator](./locator)

  This demo illustrates how to write a simple custom `Ice::Locator`.

- [mtalk](./mtalk)

  A command-line client that uses multicast to locate another device on
  Linux or Android and then chat over a secure peer-to-peer connection.

- [multicast](./multicast)

  This demo illustrates the use of UDP multicast.

- [nested](./nested)

  A demo to illustrate how nested callbacks work, and how the size of
  the thread pool affects the maximum nesting depth.

- [optional](./optional)

  This demo shows the use of the optional keyword.

- [plugin](./plugin)

  A demo to illustrate how to write and use Ice plugins.

- [properties](./properties)

  Shows how to access a server's PropertiesAdmin facet in order to
  retrieve and modify its configuration properties, and how the server
  can receive notifications whenever its properties are changed.

- [secure](./secure) TBD

- [session](./session)

  This demo shows how to use sessions to clean up client-specific
  resources in a server after the client shuts down or crashes.

- [throughput](./throughput)

  A simple throughput demo that allows you to send sequences of
  various types between client and server and to measure the maximum
  bandwidth that can be achieved using serialized synchronous
  requests.
