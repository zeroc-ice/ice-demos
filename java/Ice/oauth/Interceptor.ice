//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.Ice.interceptor"]
module Demo
{
    exception AuthorizationException {}
    exception TokenExpiredException extends AuthorizationException {}

    struct Token
    {
        string id;
        string user;
        long expireTime;
    }

    interface Authenticator
    {
        Token getToken(string username, string password);
        idempotent void validateToken(string tokenId)
            throws AuthorizationException;
    }

    interface Thermostat
    {
        idempotent float getTemp();
        idempotent void setTemp(float temp)
            throws AuthorizationException;
        void shutdown();
    }
}
