// Copyright (c) ZeroC, Inc.

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a plain proxy to the remote object.
Ice.ObjectPrx greeter = Ice.ObjectPrxHelper.createProxy(communicator, "greeter:tcp -h localhost -p 4061");

// Create an encapsulation for the input parameter (the user name).
var outputStream = new Ice.OutputStream();
outputStream.startEncapsulation();
outputStream.writeString(Environment.UserName);
outputStream.endEncapsulation();

// Send a request using ice_invokeAsync and wait for the response. ice_invokeAsync can throws exceptions such as
// Ice.ConnectionRefusedException (an Ice local exception), or Ice.ObjectNotExistException (another Ice local exception
// reported by the server).
(bool success, byte[] encapsulation) =
    await greeter.ice_invokeAsync(
        operation: "greet",
        mode: Ice.OperationMode.Normal, // as opposed to Idempotent
        inEncaps: outputStream.finished());

if (success)
{
    // Unmarshal the encapsulation to get the greeting.
    var inputStream = new Ice.InputStream(communicator, encapsulation);
    _ = inputStream.startEncapsulation();
    string greeting = inputStream.readString();
    inputStream.endEncapsulation();

    Console.WriteLine(greeting);
}
else
{
    // success = false means the encapsulation holds a user exception. This should not happen since our implementation
    // does not throw/return any user exception.
    Console.WriteLine("greet failed with an unexpected user exception");
}
