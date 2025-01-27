// Copyright (c) ZeroC, Inc.

module Demo
{
    exception RequestCanceledException
    {
    }

    interface Hello
    {
        idempotent void sayHello(int delay)
            throws RequestCanceledException;

        void shutdown();
    }
}
