// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "ClientRun.h"
#include "UtilSecureTransport.h"
#include <Ice/Ice.h>

// The main function for the SecureTransport-based client (macOS).
int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Load the trusted root CA certificate from a file using the loadCertificateFromFile helper
    // function defined in UtilSecureTransport.h. The returned CFArrayRef contains a single SecCertificateRef.
    CFArrayRef trustedRootCertificates = Util::loadCertificateFromFile("../../../certs/ca_cert.der");

    // Create client authentication options using the loaded trusted root certificate.
    // This certificate is used during the TLS handshake to validate the server's certificate chain.
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions{
        .trustedRootCertificates = trustedRootCertificates};

    // Start the secure greeter client with the provided command-line arguments and authentication options.
    // The `Client::run` function, defined in ClientRun.cpp, initializes the Ice communicator and sets up the proxy.
    // Ensure the CFArrayRef holding the trusted CAs is released after execution, even if an exception is thrown.
    try
    {
        Client::run(argc, argv, std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        CFRelease(trustedRootCertificates);
        throw;
    }

    CFRelease(trustedRootCertificates);
    return 0;
}
