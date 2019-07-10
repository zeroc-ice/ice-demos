//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.oauth"]
module Demo
{
    exception AuthorizationException {}

    interface Thermostat
    {
        idempotent float getTemp();
        idempotent void setTemp(float temp)
            throws AuthorizationException;
        void shutdown();
    }
}
