//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.multicast.Demo.*;

public class DiscoverI implements Discover
{
    DiscoverI(com.zeroc.Ice.ObjectPrx obj)
    {
        _obj = obj;
    }

    @Override
    public void lookup(DiscoverReplyPrx reply, com.zeroc.Ice.Current current)
    {
        try
        {
            reply.reply(_obj);
        }
        catch(com.zeroc.Ice.LocalException ex)
        {
            // Ignore
        }
    }

    private com.zeroc.Ice.ObjectPrx _obj;
}
