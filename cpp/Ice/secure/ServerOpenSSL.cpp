// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
    SSL_CTX* serverSSLContext = SSL_CTX_new(TLS_method());
    SSL_CTX_use_certificate_chain_file(serverSSLContext, "../../../certs/server_cert.pem");
    SSL_CTX_use_PrivateKey_file(serverSSLContext, "../../../certs/server_key.pem", SSL_FILETYPE_PEM);

    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions{
        .serverSSLContextSelectionCallback = [serverSSLContext](const string&)
        {
            SSL_CTX_up_ref(serverSSLContext);
            return serverSSLContext;
        }};
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
