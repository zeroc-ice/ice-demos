// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./Latency").Demo;

var communicator;

//
// Asynchronous loop, each call to the given function returns a
// promise that when fulfilled runs the next iteration.
//
function loop(fn, repetitions)
{
    let i = 0;
    function next()
    {
        if(i++ < repetitions)
        {
            return fn.call().then(next);
        }
    };
    return next();
}

Ice.Promise.try(
    function()
    {
        //
        // Initialize the communicator and create a proxy to the
        // ping object.
        //
        communicator = Ice.initialize(process.argv);
        const repetitions = 1000;
        const proxy = communicator.stringToProxy("ping:default -p 10000");

        //
        // Down-cast the proxy to the Demo.Ping interface.
        //
        return Demo.PingPrx.checkedCast(proxy).then(
            function(obj)
            {
                console.log("pinging server " + repetitions + " times (this may take a while)");
                const start = new Date().getTime();
                
                return loop(() => obj.ice_ping(), repetitions).then(() =>
                    {
                        //
                        // Write the results.
                        //
                        const total = new Date().getTime() - start;
                        console.log("time for " + repetitions + " pings: " + total + "ms");
                        console.log("time per ping: " + (total / repetitions) + "ms");
                    });
            });
    }
).finally(() =>
    {
        //
        // Destroy the communicator if required.
        //
        if(communicator)
        {
            return communicator.destroy();
        }
    }
).catch(ex =>
    {
        //
        // Handle any exceptions above.
        //
        console.log(ex.toString());
        process.exit(1);
    });
