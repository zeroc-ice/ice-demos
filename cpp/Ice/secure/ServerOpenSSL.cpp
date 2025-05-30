// Copyright (c) ZeroC, Inc.

#include "Server.h"
#include <Ice/Ice.h>

using namespace std;

int
main(int argc, char* argv[])
{
    // Create an OpenSSL SSL_CTX object to configure server-side SSL/TLS settings.
    SSL_CTX* serverSSLContext = SSL_CTX_new(TLS_method());

    // Set the server's certificate chain and private key.
    SSL_CTX_use_certificate_chain_file(serverSSLContext, "../../../certs/server_cert.pem");
    SSL_CTX_use_PrivateKey_file(serverSSLContext, "../../../certs/server_key.pem", SSL_FILETYPE_PEM);

    // Create server authentication options using the custom SSL_CTX configured above.
    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions{
        .serverSSLContextSelectionCallback = [serverSSLContext](const string&)
        {
            // Increase the reference count of the SSL_CTX object to ensure it remains valid for the lifetime of the
            // connection. The connection that invokes this callback takes ownership of the SSL_CTX and will free it
            // when the connection is closed.
            SSL_CTX_up_ref(serverSSLContext);
            return serverSSLContext;
        }};

    // Start the secure greeter server with the given command-line arguments and authentication options.
    // The `run` function, defined in Server.cpp, initializes the Ice object adapter and adds the Chatbot servant to it.
    // Ensure the SSL_CTX is freed after execution, regardless of success or failure.
    try
    {
        run(argc, argv, std::move(serverAuthenticationOptions));
    }
    catch (...)
    {
        SSL_CTX_free(serverSSLContext);
        throw;
    }
    SSL_CTX_free(serverSSLContext);
    return 0;
}
