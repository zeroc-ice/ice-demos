//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.context.Demo.*;

public class ContextI implements Context
{
    @Override
    public void call(com.zeroc.Ice.Current current)
    {
        System.out.print("Type = ");
        String type = current.ctx.get("type");
        if(type != null)
        {
            System.out.print(type);
        }
        else
        {
            System.out.print("None");
        }
        System.out.println();
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
