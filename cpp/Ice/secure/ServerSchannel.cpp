// Copyright (c) ZeroC, Inc.

#include "Server.h"
#include "UtilSchannel.h"
#include <Ice/Ice.h>

using namespace std;

int
main(int argc, char* argv[])
{
    PCCERT_CONTEXT serverCertificate = certificateContextFromPKCS12File("../../../certs/server.p12", "password");

    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions;
    serverAuthenticationOptions.serverCredentialsSelectionCallback = [serverCertificate](const string&)
    {
        CertDuplicateCertificateContext(serverCertificate);
        SCH_CREDENTIALS credentials{};
        credentials.dwVersion = SCH_CREDENTIALS_VERSION;
        credentials.cCreds = 1;
        credentials.paCred = const_cast<PCCERT_CONTEXT*>(&serverCertificate);
        return credentials;
    };

    try
    {
        run(argc, argv, std::move(serverAuthenticationOptions));
    }
    catch (...)
    {
        CertFreeCertificateContext(serverCertificate);
        throw;
    }
    CertFreeCertificateContext(serverCertificate);
    return 0;
}
