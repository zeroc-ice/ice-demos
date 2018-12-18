// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import {Ice} from "ice";
import {Demo} from "./generated";

(async () =>
{
    let communicator;
    try
    {
        communicator = Ice.initialize(process.argv);
        if(process.argv.length > 2)
        {
            throw new Error("too many arguments");
        }
        const repetitions = 1000;
        //
        // Create a proxy to the ping object and down-cast the proxy to the
        // Demo.Ping interface
        //
        const proxy = await Demo.PingPrx.checkedCast(communicator.stringToProxy("ping:default -p 10000"));

        console.log(`pinging server ${repetitions} times (this may take a while)`);
        const start = new Date().getTime();

        for(let i = 0; i < repetitions; ++i)
        {
            await proxy.ice_ping();
        }
        const total = new Date().getTime() - start;
        console.log(`time for ${repetitions} pings: ${total}ms`);
        console.log(`time per ping: ${total / repetitions}ms`);
    }
    catch(ex)
    {
        console.log(ex.toString());
        process.exitCode = 1;
    }
    finally
    {
        if(communicator)
        {
            await communicator.destroy();
        }
    }
})();
