//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    class Nested : INested
    {
        private readonly INestedPrx _self;
        
        public Nested(INestedPrx self) =>  _self = self;

        public void NestedCall(int level, INestedPrx proxy, Current current)
        {
            Console.WriteLine($"{level}");
            if (--level > 0)
            {
                // Ensure the invocation times out if the nesting level is too high and there are no more threads in
                // the thread pool to dispatch the call.
                using var cancel = new CancellationTokenSource(5000);
                proxy.NestedCall(level, _self, cancel: cancel.Token);
            }
        }
    }

}
