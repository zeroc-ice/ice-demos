//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

module Demo
{
    exception InvalidValue
    {
        string message;
    }

    interface Fibonacci
    {
        // Computes a fibonacci number while maintaining a cache of previously requested values to speed up responses.
        idempotent int compute(int n) throws InvalidValue;

        // Clears the cached values.
        void clearCache();

        // Shuts down the server. That's ok for a demo - a real server would typically never allow its clients to shut
        // it down.
        void shutdown();
    }
}
