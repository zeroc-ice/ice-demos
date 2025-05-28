// Copyright (c) ZeroC, Inc.

#include "Client.h"
#include "UtilSchannel.h"
#include <Ice/Ice.h>

int
main(int argc, char* argv[])
{
    HCERTSTORE trustedRootCertificates = loadCertificateStore("../../../certs/ca_cert.der");
    Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions;
    clientAuthenticationOptions.trustedRootCertificates = trustedRootCertificates;

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
