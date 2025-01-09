//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.hello;

import com.zeroc.Ice.ObjectPrx;

enum DeliveryMode
{
    TWOWAY,
    TWOWAY_SECURE,
    ONEWAY,
    ONEWAY_BATCH,
    ONEWAY_SECURE,
    ONEWAY_SECURE_BATCH,
    DATAGRAM,
    DATAGRAM_BATCH;

    public boolean isOneway()
    {
        return this == ONEWAY || this == ONEWAY_SECURE || this == DATAGRAM;
    }

    public boolean isBatch()
    {
        return this == ONEWAY_BATCH || this == DATAGRAM_BATCH || this == ONEWAY_SECURE_BATCH;
    }
}
