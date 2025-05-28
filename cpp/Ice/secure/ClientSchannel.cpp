// Copyright (c) ZeroC, Inc.

#include "Client.h"
#include <Ice/Ice.h>

#ifdef ICE_USE_SCHANNEL
int
main(int argc, char* argv[])
{
    HCERTSTORE trustedRootCertificates = loadTrustedRootCertificates("ca_cert.der");
    Ice::SSL::ClientAuthenticationOptions{.trustedRootCertificates = trustedRootCertificates};
    try
    {
        greet(std::move(clientAuthenticationOptions));
    }
    catch (...)
    {
        CertCloseStore(trustedRootCertificates, 0);
        throw;
    }
    CertCloseStore(trustedRootCertificates, 0);
    return 0;
}
#endif