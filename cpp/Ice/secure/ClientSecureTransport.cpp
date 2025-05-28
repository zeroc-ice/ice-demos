
#include "Client.h"
#include "UtilSecureTransport.h"
#include <Ice/Ice.h>

int
main(int argc, char* argv[])
{
    // Load the trusted root CA certificate from a file using the loadCertificateFromFile helper
    // function defined in UtilSecureTransport.h.
    CFArrayRef trustedRootCertificates = loadCertificateFromFile("../../../certs/ca_cert.der");

    // Create client authentication options using the trusted root certificates.
    // These certificates are used to validate the server's certificate chain during the SSL handshake.
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions{
        .trustedRootCertificates = trustedRootCertificates};

    // Start the secure greeter client with the given command-line arguments and authentication options.
    // The `run` function, defined in Client.cpp, initializes the Ice communicator and creates the proxy.
    // Ensure the CFArrayRef holding the trusted CAs is released after execution, regardless of success or failure.
    try
    {
        run(argc, argv, std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        CFRelease(trustedRootCertificates);
        throw;
    }
    CFRelease(trustedRootCertificates);
    return 0;
}
