// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include "Server.h"
#include "UtilSecureTransport.h"

using namespace std;

int main(int argc, char* argv[])
{
    CFArrayRef serverIdentity = loadSecIdentityWithLabel("ca.server");

    Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions{
        .serverCertificateSelectionCallback = [serverIdentity](const string&)
        {
            CFRetain(serverIdentity);
            return serverIdentity;
        }};
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
