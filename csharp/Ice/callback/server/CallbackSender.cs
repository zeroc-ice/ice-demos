// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public sealed class CallbackSender : ICallbackSender
    {
        public void InitiateCallback(ICallbackReceiverPrx? proxy, Current current, CancellationToken cancel)
        {
            Console.Out.WriteLine("initiating callback");
            try
            {
                proxy!.Callback();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }

        public void Shutdown(Current current, CancellationToken cancel)
        {
            Console.Out.WriteLine("Shutting down...");
            try
            {
                current.Communicator.ShutdownAsync();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
    }

}
