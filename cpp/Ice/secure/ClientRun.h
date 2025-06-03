// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>

namespace Client
{
    /// Starts a secure greeter client using the provided command-line arguments and client authentication options.
    /// @param argc The number of command-line arguments.
    /// @param argv The array of command-line arguments.
    /// @param clientAuthenticationOptions The client authentication options used to initialize the Ice communicator
    /// for secure connections.
    void run(int argc, char* argv[], Ice::SSL::ClientAuthenticationOptions clientAuthenticationOptions);
}
