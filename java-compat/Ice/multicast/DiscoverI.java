// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class DiscoverI extends _DiscoverDisp
{
    DiscoverI(Ice.ObjectPrx obj)
    {
        _obj = obj;
    }

    @Override
    public void
    lookup(DiscoverReplyPrx reply, Ice.Current current)
    {
        try
        {
            reply.reply(_obj);
        }
        catch(Ice.LocalException ex)
        {
            // Ignore
        }
    }

    private Ice.ObjectPrx _obj;
}
