// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "ClientRun.h"
#include <Ice/Ice.h>

using namespace std;

// The main function for the OpenSSL-based client (Linux).
int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Create an OpenSSL SSL_CTX object to configure client-side TLS settings.
    SSL_CTX* clientSSLContext = SSL_CTX_new(TLS_method());
    if (!clientSSLContext)
    {
        throw runtime_error("Unable to create SSL context");
    }

    // Load the CA certificate used to verify the server's certificate.
    // This sets the trusted root for the SSL_CTX.
    if (!SSL_CTX_load_verify_file(clientSSLContext, "../../../certs/ca_cert.pem"))
    {
        SSL_CTX_free(clientSSLContext);
        throw runtime_error("Unable to load CA certificate file");
    }

    // Create client authentication options using the configured SSL_CTX.
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions{
        .clientSSLContextSelectionCallback = [clientSSLContext](const string&)
        {
            // Increase the reference count to ensure the SSL_CTX remains valid for the lifetime of the connection.
            // The connection that invokes this callback takes ownership and will free the SSL_CTX when closed.
            SSL_CTX_up_ref(clientSSLContext);
            return clientSSLContext;
        }};

    // Start the secure greeter client with the provided command-line arguments and authentication options.
    // The `Client::run` function, defined in ClientRun.cpp, initializes the Ice communicator and sets up the proxy.
    // Ensure the SSL_CTX is released after use, even if an exception is thrown.
    try
    {
        Client::run(argc, argv, std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        SSL_CTX_free(clientSSLContext);
        throw;
    }

    SSL_CTX_free(clientSSLContext);
    return 0;
}
