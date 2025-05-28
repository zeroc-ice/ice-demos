// Copyright (c) ZeroC, Inc.

#include "Server.h"
#include "UtilSecureTransport.h"
#include <Ice/Ice.h>

using namespace std;

int
main(int argc, char* argv[])
{
    // Load the server's identity (certificate and private key) from the keychain.
    // The identity must have been previously imported, as explained in the README.md file.
    //
    // This function searches the user's keychain search list and returns the first identity with a matching subject.
    // The result is a CFArrayRef containing a single SecIdentityRef, suitable for use with SSLSetCertificate.
    CFArrayRef serverIdentity = loadSecIdentityWithMatchingSubject("ca.server");

    // Create the server authentication options with a certificate selection callback that returns the server identity.
    // The callback is invoked by the SSL transport for each new incoming connection before the SSL handshake begins.
    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions{
        .serverCertificateSelectionCallback = [serverIdentity](const string&)
        {
            // Increase the reference count of the SecIdentityRef to ensure it remains valid for the lifetime of the
            // connection. The connection that invokes this callback takes ownership of the SecIdentityRef and will
            // release it when the connection is closed.
            CFRetain(serverIdentity);
            return serverIdentity;
        }};

    // Start the secure greeter server with the given command-line arguments and authentication options.
    // The `run` function, defined in Server.cpp, initializes the Ice object adapter and adds the Chatbot servant to it.
    // Ensure CFArrayRef holding the server identity is released after execution, regardless of success or failure.
    try
    {
        run(argc, argv, std::move(serverAuthenticationOptions));
    }
    catch (...)
    {
        CFRelease(serverIdentity);
        throw;
    }
    CFRelease(serverIdentity);
    return 0;
}
