// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "ClientRun.h"
#include "UtilSchannel.h"
#include <Ice/Ice.h>

// The main function for the Schannel-based client (Windows).
int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Load the trusted root CA certificate from a file using the loadCertificateStore helper function
    // defined in UtilSchannel.h. This certificate will be used to validate the server's certificate chain.
    HCERTSTORE trustedRootCertificates = Util::loadCertificateStore("../../../certs/ca_cert.der");

    // Create the client authentication options with the loaded trusted root certificates.
    // These certificates will be used during the TLS handshake to verify the server's identity.
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions;
    clientAuthenticationOptions.trustedRootCertificates = trustedRootCertificates;

    // Start the secure greeter client with the provided command-line arguments and authentication options.
    // The `Client::run` function, defined in ClientRun.cpp, initializes the Ice communicator and creates the proxy.
    // Ensure the certificate store is closed after execution, even if an exception occurs.
    try
    {
        Client::run(argc, argv, std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        CertCloseStore(trustedRootCertificates, 0);
        throw;
    }

    CertCloseStore(trustedRootCertificates, 0);
    return 0;
}
