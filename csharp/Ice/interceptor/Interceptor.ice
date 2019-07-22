//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

module Demo
{
    exception AuthorizationException {}
    exception TokenExpiredException extends AuthorizationException {}

    struct Token
    {
        string value;
        string username;
        long expireTime;
    }

    interface Authenticator
    {
        Token getToken(string username, string password);
    }

    interface Thermostat
    {
        idempotent float getTemp();
        idempotent void setTemp(float temp)
            throws AuthorizationException;
    }
}
