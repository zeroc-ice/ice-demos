// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public class DiscoverI : DiscoverDisp_
{
    public
    DiscoverI(Ice.ObjectPrx obj)
    {
        _obj = obj;
    }

    public override void
    lookup(DiscoverReplyPrx reply, Ice.Current current)
    {
        try
        {
            reply.reply(_obj);
        }
        catch(Ice.LocalException)
        {
            // Ignore
        }
    }

    private Ice.ObjectPrx _obj;
}
