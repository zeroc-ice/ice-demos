// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class DiscoverReplyI extends _DiscoverReplyDisp
{
    @Override
    public synchronized void
    reply(Ice.ObjectPrx obj, Ice.Current current)
    {
        if(_obj == null)
        {
            _obj = obj;
        }
        notify();
    }

    public synchronized Ice.ObjectPrx
    waitReply(long timeout)
    {
        long end = System.currentTimeMillis() + timeout;
        while(_obj == null)
        {
            long delay = end - System.currentTimeMillis();
            if(delay > 0)
            {
                try
                {
                    wait(delay);
                }
                catch(java.lang.InterruptedException ex)
                {
                }
            }
            else
            {
                break;
            }
        }
        return _obj;
    }

    private Ice.ObjectPrx _obj;
}
