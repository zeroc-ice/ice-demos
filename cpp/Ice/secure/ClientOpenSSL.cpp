// Copyright (c) ZeroC, Inc.

#include "Client.h"
#include <Ice/Ice.h>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an OpenSSL SSL_CTX object to configure client-side SSL/TLS settings.
    SSL_CTX* clientSSLContext = SSL_CTX_new(TLS_method());
    if (!clientSSLContext)
    {
        throw runtime_error("Unable to create SSL context");
    }

    // Set the test CA certificate file as the trusted CA for the SSL_CTX object.
    if (!SSL_CTX_load_verify_file(clientSSLContext, "../../../certs/ca_cert.pem"))
    {
        SSL_CTX_free(clientSSLContext);
        throw runtime_error("Unable to load CA certificate file");
    }

    // Create client authentication options using the custom SSL_CTX configured above.
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions{
        .clientSSLContextSelectionCallback = [clientSSLContext](const string&)
        {
            // Increase the reference count of the SSL_CTX object to ensure it remains valid for the lifetime of the
            // connection. The connection that invokes this callback takes ownership of the SSL_CTX and will free it
            // when the connection is closed.
            SSL_CTX_up_ref(clientSSLContext);
            return clientSSLContext;
        }};

    // Start the secure greeter client with the given command-line arguments and authentication options.
    // The `run` function, defined in Client.cpp, initializes the Ice communicator and creates the proxy.
    // Ensure the SSL_CTX is freed after execution, regardless of success or failure.
    try
    {
        run(argc, argv, std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        SSL_CTX_free(clientSSLContext);
        throw;
    }
    SSL_CTX_free(clientSSLContext);
    return 0;
}
