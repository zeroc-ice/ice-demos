Demos in this directory:

- [async](./async)

  This demo illustrates the use of Asynchronous Method Invocation
  (AMI) and Asynchronous Method Dispatch (AMD).

- [asyncInvocation](./asyncInvocation)

  This demo illustrates the use of Asynchronous Method Invocation
  (AMI).

- [bidir](./bidir)

  This demo shows how to use bidirectional connections for callbacks.
  This is typically used if the server cannot open a connection to the
  client to send callbacks, for example, because firewalls block
  incoming connections to the client.

- [context](./context)

  This demo shows how to use Ice request contexts.

- [hello](./hello)

  This demo illustrates how to invoke ordinary (twoway) operations, as
  well as how to invoke oneway operations, use datagrams, secure
  invocations, and how to use batched invocations.

- [interceptor](./interceptor)

  This demo shows how to use dispatch interceptors to secure operations
  with a token based authentication system, in addition to the use of
  implicit contexts.

- [invoke](./invoke)

  This demo illustrates the use of the Ice streaming API.

- [latency](./latency)

  A simple latency test that measures the basic call dispatch delay of
  Ice.

- [minimal](./minimal)

  This demo illustrates a minimal Ice application.

- [optional](./optional)

  This demo shows the use of the optional keyword.

- [throughput](./throughput)

  A simple throughput demo that allows you to send sequences of
  various types between client and server and to measure the maximum
  bandwidth that can be achieved using serialized synchronous requests.
