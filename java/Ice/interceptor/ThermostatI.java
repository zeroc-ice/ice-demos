//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

public class ThermostatI implements Thermostat
{
    @Override
    public float getTemp(com.zeroc.Ice.Current current)
    {
        return temperature;
    }

    @Override
    //
    // Even though the actual operation never throws an AuthorizationException
    // it's still required since the dispatch interceptor that checks for
    // access authorization can throw it. To the client however, it will see
    // the exception as if the operation threw it itself, hence why the
    // exception must be specified here.
    //
    public void setTemp(float temp, com.zeroc.Ice.Current current)
        throws AuthorizationException
    {
        temperature = temp;
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    private float temperature = 23.5f;
}
