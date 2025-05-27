// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include "Client.h"

using namespace std;

#if defined(ICE_USE_OPENSSL)
int main(int argc, char* argv[])
{
    // Create an OpenSSL SSL_CTX object for the client and configure it to use the CA certificate file
    // for verifying the server's certificate.

    SSL_CTX* clientSSLContext = SSL_CTX_new(TLS_method());
    if (!clientSSLContext)
    {
        throw runtime_error("Unable to create SSL context");
    }

    if (!SSL_CTX_load_verify_file(clientSSLContext, "../../../certs/ca_cert.pem"))
    {
        SSL_CTX_free(clientSSLContext);
        throw runtime_error("Unable to load CA certificate file");
    }

    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions{
        .clientSSLContextSelectionCallback = [clientSSLContext](const string&)
        {
            SSL_CTX_up_ref(clientSSLContext);
            return clientSSLContext;
        }
    };

    try
    {
        greet(std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        SSL_CTX_free(clientSSLContext);
        throw;
    }
    SSL_CTX_free(clientSSLContext);
    return 0;
}
#endif
