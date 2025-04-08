// Copyright (c) ZeroC, Inc.

using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
// Slice module VisitorCenter in Greeter.ice maps to C# namespace VisitorCenter.
using VisitorCenter;

// Create the authentication options with a custom certificate validation callback that uses the test certificates'
// Root CA included with the demos.
using var rootCA = new X509Certificate2("../../../../certs/cacert.der");
var clientAuthenticationOptions = new SslClientAuthenticationOptions()
{
    // The remote certificate validation callback is invoked when the client receives a server certificate during the
    // SSL/TLS handshake. The callback must return true if the certificate is considered valid and the connection
    // should proceed; otherwise, it must return false.
    RemoteCertificateValidationCallback = (sender, certificate, chain, errors) =>
    {
        if (certificate is X509Certificate2 peerCertificate)
        {
            // Create a custom X509Chain object to validate the server certificate.
            // We configure a custom trust store that includes only the root CA certificate used by the demo.
            // This ensures that the client only accepts certificates signed by our custom CA.
            using var customChain = new X509Chain();
            customChain.ChainPolicy.RevocationMode = X509RevocationMode.NoCheck;
            customChain.ChainPolicy.DisableCertificateDownloads = true;

            // This ensures that only the root CA from the custom trust store is used for validation.
            customChain.ChainPolicy.TrustMode = X509ChainTrustMode.CustomRootTrust;
            customChain.ChainPolicy.CustomTrustStore.Add(rootCA);

            // Build returns true if the server certificate is valid according to the custom chain policy.
            return customChain.Build(peerCertificate);
        }
        else
        {
            // Return false if the certificate is not of the expected type.
            return false;
        }
    }
};

var initData = new Ice.InitializationData();
initData.properties = new Ice.Properties(ref args);
// Set the client authentication options that the communicator will use for configuring SSL outgoing connections.
initData.clientAuthenticationOptions = clientAuthenticationOptions;

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using Ice.Communicator communicator = Ice.Util.initialize(initData);

// GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a "stringified
// proxy" with the address of the target object.
// If you run the server on a different computer, replace localhost in the string below with the server's hostname
// or IP address.
GreeterPrx greeter = GreeterPrxHelper.createProxy(communicator, "greeter:ssl -h localhost -p 4061");

// Send a request to the remote object and get the response.
string greeting = await greeter.GreetAsync(Environment.UserName);

Console.WriteLine(greeting);
