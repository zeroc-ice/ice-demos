// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class DispatchInterceptorPlugin : IPlugin
    {
        public void Initialize(PluginInitializationContext context)
        {
            context.AddDispatchInterceptor((request, current, next) =>
            {
                string userName = request.BinaryContext[100].Read(istr => istr.ReadString());
                current.Communicator.Logger.Print(
                    $"Dispatching operation: {current.Operation} invoke by user: {userName}");
                return next(request, current);
            });
        }

        public ValueTask DisposeAsync() => default;
    }

    public class DispatchInterceptorPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) =>
            new DispatchInterceptorPlugin();
    }
}
