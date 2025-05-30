// Copyright (c) ZeroC, Inc.

#include "Client.h"
#include "UtilSchannel.h"
#include <Ice/Ice.h>

int
main(int argc, char* argv[])
{
    // Load the trusted root CA certificate from a file using the loadCertificateStore helper function defined in
    // UtilSchannel.h.
    HCERTSTORE trustedRootCertificates = loadCertificateStore("../../../certs/ca_cert.der");

    // Create the client authentication options using the trusted root certificates.
    // These certificates are used to validate the server's certificate chain during the SSL handshake.
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions;
    clientAuthenticationOptions.trustedRootCertificates = trustedRootCertificates;

    // Start the secure greeter client with the given command-line arguments and authentication options.
    // The `run` function, defined in Client.cpp, initializes the Ice communicator and creates the proxy.
    // Ensure the certificate store containing the trusted CAs is closed after execution, regardless of success or
    // failure.
    try
    {
        run(argc, argv, std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        CertCloseStore(trustedRootCertificates, 0);
        throw;
    }
    CertCloseStore(trustedRootCertificates, 0);
    return 0;
}
