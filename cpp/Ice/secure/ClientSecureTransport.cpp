
#include "Client.h"
#include "UtilSecureTransport.h"
#include <Ice/Ice.h>

int
main(int argc, char* argv[])
{
    // Configure the the trusted root certificates that will be used to validate the server's certificate.
    CFArrayRef trustedRootCertificates = loadCertificateFromFile("../../../certs/ca_cert.der");
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions{
        .trustedRootCertificates = trustedRootCertificates};
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
