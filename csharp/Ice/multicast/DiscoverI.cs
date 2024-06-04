// Copyright (c) ZeroC, Inc.

using Demo;

internal class DiscoverI(Ice.ObjectPrx obj) : DiscoverDisp_
{
    public override void
    lookup(DiscoverReplyPrx reply, Ice.Current current)
    {
        try
        {
            reply.reply(_obj);
        }
        catch (Ice.LocalException)
        {
            // Ignore
        }
    }

    private Ice.ObjectPrx _obj = obj;
}
