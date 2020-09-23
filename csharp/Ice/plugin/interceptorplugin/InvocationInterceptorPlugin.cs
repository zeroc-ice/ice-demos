// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class InvocationInterceptorPlugin : IPlugin
    {
        public void Initialize(PluginInitializationContext context)
        {
            context.AddInvocationInterceptor((prx, request, next) =>
            {
                request.AddBinaryContextEntry(100, Environment.UserName, (ostr, value) => ostr.WriteString(value));
                return next(prx, request);
            });
        }

        public ValueTask DisposeAsync() => default;
    }

    public class InvocationInterceptorPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) =>
            new InvocationInterceptorPlugin();
    }
}
