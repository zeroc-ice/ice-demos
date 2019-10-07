//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.interceptor"]
module Demo
{
    exception AuthorizationException
    {
    }

    exception TokenExpiredException extends AuthorizationException
    {
    }

    interface Authenticator
    {
        string getToken();
    }

    interface Thermostat
    {
        idempotent float getTemp();

        //
        // Even though the actual operation never throws an AuthorizationException
        // it's still required since the dispatch interceptor that checks for
        // access authorization can throw it. To the client however, it will see
        // the exception as if the operation threw it itself, hence why the
        // exception must be specified here.
        //
        idempotent void setTemp(float temp) throws AuthorizationException;
        void shutdown() throws AuthorizationException;
    }
}
