//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

module Demo
{
    exception AuthorizationException {}
    exception TokenExpiredException extends AuthorizationException {}

    interface Authenticator
    {
        string getToken();
    }

    interface Thermostat
    {
        idempotent float getTemp();
        idempotent void setTemp(float temp)
            throws AuthorizationException;
        void shutdown()
            throws AuthorizationException;
    }
}
