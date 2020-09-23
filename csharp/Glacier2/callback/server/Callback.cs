// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public sealed class Callback : ICallback
    {
        public void InitiateCallback(ICallbackReceiverPrx? proxy, Current current)
        {
            Console.WriteLine($"initiating callback to: {proxy}");
            try
            {
                proxy!.Callback();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }

        public void Shutdown(Current current)
        {
            Console.WriteLine("shutting down...");
            current.Communicator.DisposeAsync();
        }
    }

}
