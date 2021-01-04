//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
