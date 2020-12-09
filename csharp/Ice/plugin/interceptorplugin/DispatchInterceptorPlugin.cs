// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Collections.Immutable;
using System.Threading;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class DispatchInterceptorPlugin : IPlugin
    {
        public Task ActivateAsync(CancellationToken cancel) => Task.CompletedTask;

        public ValueTask DisposeAsync() => default;

        internal DispatchInterceptorPlugin(Communicator communicator) =>
            communicator.DefaultDispatchInterceptors = communicator.DefaultDispatchInterceptors.ToImmutableList().Add(
                (request, current, next, cancel) =>
                {
                    string userName = request.BinaryContext[100].Read(istr => istr.ReadString());
                    current.Communicator.Logger.Print(
                        $"Dispatching operation: {current.Operation} invoke by user: {userName}");
                    return next(request, current, cancel);
                });
    }

    public class DispatchInterceptorPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) =>
            new DispatchInterceptorPlugin(communicator);
    }
}
