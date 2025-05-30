// Copyright (c) ZeroC, Inc.

#include "Server.h"
#include "UtilSchannel.h"
#include <Ice/Ice.h>

using namespace std;

int
main(int argc, char* argv[])
{
    // Load the server's certificate context (certificate and private key) from a PKCS12 file.
    PCCERT_CONTEXT serverCertificate = certificateContextFromPKCS12File("../../../certs/server.p12", "password");

    // Create the server authentication options with a credentials selection callback that returns a SCH_CREDENTIALS
    // structure using the server's certificate context.
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

    // Start the secure greeter server with the given command-line arguments and authentication options.
    // The `run` function, defined in Server.cpp, initializes the Ice object adapter and adds the Chatbot servant to it.
    // Ensure the server certificate context is released after execution, regardless of success or failure.
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
