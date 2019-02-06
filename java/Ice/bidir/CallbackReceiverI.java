//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.bidir.Demo.*;

class CallbackReceiverI implements CallbackReceiver
{
    CallbackReceiverI()
    {
    }

    @Override
    public void callback(int num, com.zeroc.Ice.Current current)
    {
        System.out.println("received callback #" + num);
    }
}
