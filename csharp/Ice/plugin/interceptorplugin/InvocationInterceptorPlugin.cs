// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Collections.Immutable;
using System.Threading;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class InvocationInterceptorPlugin : IPlugin
    {
        public Task ActivateAsync(CancellationToken cancel) => Task.CompletedTask;

        public ValueTask DisposeAsync() => default;

        internal InvocationInterceptorPlugin(Communicator communicator) =>
             communicator.DefaultInvocationInterceptors = ImmutableList.Create<InvocationInterceptor>(
                (prx, request, next, cancel) =>
                {
                    request.AddBinaryContextEntry(100, Environment.UserName, (ostr, value) => ostr.WriteString(value));
                    return next(prx, request, cancel);
                }).AddRange(communicator.DefaultInvocationInterceptors);
    }

    public class InvocationInterceptorPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) =>
            new InvocationInterceptorPlugin(communicator);
    }
}
