// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>

namespace Server
{
    /// Starts a secure greeter server using the specified command-line arguments and server authentication options.
    /// @param argc The number of command-line arguments.
    /// @param argv The array of command-line arguments.
    /// @param serverAuthenticationOptions The server authentication options used to initialize the Ice object adapter
    /// for secure connections.
    void run(int argc, char* argv[], Ice::SSL::ServerAuthenticationOptions serverAuthenticationOptions);
}
