// Copyright (c) ZeroC, Inc.

using Ice;

namespace InvokeServer;

/// <summary>A Chatbot is an Ice servant that implements operation greet directly, without generated code.</summary>
internal class Chatbot : Ice.Object
{
    // Implements abstract method dispatchAsync from the Object interface. There is no synchronous version of dispatch.
    public ValueTask<OutgoingResponse> dispatchAsync(IncomingRequest request)
    {
        if (request.current.operation == "greet")
        {
            // Unmarshal the input parameter (the user name).
            request.inputStream.startEncapsulation();
            string name = request.inputStream.readString();
            request.inputStream.endEncapsulation();

            Console.WriteLine($"Dispatching greet request {{ name = '{name}' }}");

            // Create an Ice encapsulation for the return value (the greeting).
            var outputStream = request.current.startReplyStream(); // for reply status Ok
            outputStream.startEncapsulation();
            outputStream.writeString($"Hello, {name}!");
            outputStream.endEncapsulation();

            // Return the response wrapped in a ValueTask.
            return new(new OutgoingResponse(outputStream));
        }
        else
        {
            // Chatbot only implements greet. It could also implement ice_ping, ice_isA and other ice_ operations, but
            // here we decided not to implement them.
            throw new OperationNotExistException();
        }
    }
}
