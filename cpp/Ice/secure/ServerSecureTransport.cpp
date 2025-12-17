// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "ServerRun.h"
#include "UtilSecureTransport.h"
#include <Ice/Ice.h>

using namespace std;

// The main function for the SecureTransport-based server (macOS).
int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // Load the server's identity (certificate and private key) from the keychain.
    // The identity must have been previously imported, as explained in the README.md file.
    //
    // This function searches the user's keychain search list and returns the first identity whose subject matches
    // the specified string. The result is a CFArrayRef containing a single SecIdentityRef, suitable for use with
    // SSLSetCertificate.
    CFArrayRef serverIdentity = Util::loadSecIdentityWithMatchingSubject("ca.server");

    // Create the server authentication options with a certificate selection callback that returns the server identity.
    // This callback is invoked by the SSL transport for each new incoming connection before the TLS handshake begins.
    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions{
        .serverCertificateSelectionCallback = [serverIdentity](const string&)
        {
            // Retain the server identity to ensure it remains valid for the duration of the connection.
            // The connection takes ownership of the CFArrayRef and will release it when the connection is closed.
            CFRetain(serverIdentity);
            return serverIdentity;
        }};

    // Start the secure greeter server with the provided command-line arguments and authentication options.
    // The `Server::run` function, defined in ServerRun.cpp, initializes the Ice object adapter and adds the Chatbot
    // servant to it. Ensure the CFArrayRef holding the server identity is released after execution, even if an
    // exception is thrown.
    try
    {
        Server::run(argc, argv, std::move(serverAuthenticationOptions));
    }
    catch (...)
    {
        CFRelease(serverIdentity);
        throw;
    }

    CFRelease(serverIdentity);
    return 0;
}
