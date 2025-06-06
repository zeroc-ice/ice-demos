// Copyright (c) ZeroC, Inc.

#include "ServerRun.h"
#include "UtilSchannel.h"
#include <Ice/Ice.h>

using namespace std;

// The main function for the Schannel-based server (Windows).
int
main(int argc, char* argv[])
{
    // Load the server's certificate and private key from a PKCS#12 file using the helper function
    // defined in UtilSchannel.h. The returned context includes both the certificate and its associated key.
    PCCERT_CONTEXT serverCertificate = Util::certificateContextFromPKCS12File("../../../certs/server.p12", L"password");

    // Create server authentication options with a credentials selection callback that provides a SCH_CREDENTIALS
    // structure referencing the loaded certificate context.
    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions;
    serverAuthenticationOptions.serverCredentialsSelectionCallback = [serverCertificate](const string&)
    {
        // Duplicate the certificate context to ensure it remains valid for the lifetime of the connection. The
        // connection that invokes this callback takes ownership of the certificate context and will free it when the
        // connection is closed.
        CertDuplicateCertificateContext(serverCertificate);

        // Populate the SCH_CREDENTIALS structure with a single certificate.
        SCH_CREDENTIALS credentials{};
        credentials.dwVersion = SCH_CREDENTIALS_VERSION;
        credentials.cCreds = 1;
        credentials.paCred = const_cast<PCCERT_CONTEXT*>(&serverCertificate);
        return credentials;
    };

    // Start the secure greeter server with the provided command-line arguments and authentication options.
    // The `Server::run` function, defined in ServerRun.cpp, initializes the Ice object adapter and adds the Chatbot
    // servant to it. Ensure the certificate context is freed after execution, even if an exception is thrown.
    try
    {
        Server::run(argc, argv, std::move(serverAuthenticationOptions));
    }
    catch (...)
    {
        CertFreeCertificateContext(serverCertificate);
        throw;
    }

    CertFreeCertificateContext(serverCertificate);
    return 0;
}
