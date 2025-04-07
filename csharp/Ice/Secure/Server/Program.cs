// Copyright (c) ZeroC, Inc.

// Create an Ice communicator. We'll use this communicator to create an object adapter.
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;

await using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create the authentication options using the test server certificate included with the demos.
var sslAuthenticationOptions = new SslServerAuthenticationOptions
{
    ServerCertificateContext = SslStreamCertificateContext.Create(
        X509CertificateLoader.LoadPkcs12FromFile(
            "../../../../certs/server.p12",
            password: "password",
            keyStorageFlags: X509KeyStorageFlags.Exportable),
        additionalCertificates: null)
};

// Create an object adapter that listens for incoming requests and dispatches them to servants.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
    "GreeterAdapter",
    "ssl -p 4063",
    sslAuthenticationOptions);

// Register the Chatbot servant with the adapter.
adapter.add(new Server.Chatbot(), Ice.Util.stringToIdentity("greeter"));

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on port 4063...");

// Shut down the communicator when the user presses Ctrl+C.
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true; // don't terminate the process
    Console.WriteLine("Caught Ctrl+C, shutting down...");
    communicator.shutdown(); // starts shutting down
};

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted;
