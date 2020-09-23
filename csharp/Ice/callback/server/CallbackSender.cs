// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public sealed class CallbackSender : ICallbackSender
    {
        public void InitiateCallback(ICallbackReceiverPrx? proxy, Current current)
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

        public void Shutdown(Current current)
        {
            Console.Out.WriteLine("Shutting down...");
            try
            {
                current.Communicator.DisposeAsync();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
    }

}
