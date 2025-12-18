// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "ServerRun.h"
#include <Ice/Ice.h>

using namespace std;

// The main function for the OpenSSL-based server (Linux).
int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Create an OpenSSL SSL_CTX object to configure server-side TLS settings.
    SSL_CTX* serverSSLContext = SSL_CTX_new(TLS_method());

    // Load the server's certificate chain and private key into the SSL context.
    // The certificate chain file should include the server certificate followed by any intermediate CAs.
    SSL_CTX_use_certificate_chain_file(serverSSLContext, "../../../certs/server_cert.pem");
    SSL_CTX_use_PrivateKey_file(serverSSLContext, "../../../certs/server_key.pem", SSL_FILETYPE_PEM);

    // Create server authentication options using the configured SSL_CTX.
    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions{
        .serverSSLContextSelectionCallback = [serverSSLContext](const string&)
        {
            // Increase the reference count of the SSL_CTX to ensure it remains valid for the lifetime of the
            // connection. The connection that invokes this callback takes ownership of the SSL_CTX and will free it
            // when closed.
            SSL_CTX_up_ref(serverSSLContext);
            return serverSSLContext;
        }};

    // Start the secure greeter server with the provided command-line arguments and authentication options.
    // The `Server::run` function, defined in ServerRun.cpp, initializes the Ice object adapter and adds the Chatbot
    // servant to it. Ensure the SSL_CTX is freed after execution, even if an exception is thrown.
    try
    {
        Server::run(argc, argv, std::move(serverAuthenticationOptions));
    }
    catch (...)
    {
        SSL_CTX_free(serverSSLContext);
        throw;
    }

    SSL_CTX_free(serverSSLContext);
    return 0;
}
