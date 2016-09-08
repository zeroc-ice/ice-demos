// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

(function(){

//
// Initialize the communicator
//
const communicator = Ice.initialize();

//
// Run the latency test.
//
function run()
{
    const hostname = document.location.hostname || "127.0.0.1";
    const secure = document.location.protocol.indexOf("https") != -1;
    const ref = secure ?
        "ping:wss -h " + hostname + " -p 9090 -r /demowss" :
        "ping:ws -h " + hostname + " -p 8080 -r /demows";
    const repetitions = 1000;

    //
    // Create a proxy to the ping object and down-cast the proxy
    // to the Demo.Ping interface.
    //
    return Demo.PingPrx.checkedCast(communicator.stringToProxy(ref)).then(ping =>
        {
            writeLine("pinging server " + repetitions + " times (this may take a while)");
            const start = new Date().getTime();
            return loop(() => ping.ice_ping(), repetitions).then(() =>
                {
                    //
                    // Write the results.
                    //
                    const total = new Date().getTime() - start;
                    writeLine("time for " + repetitions + " pings: " + total + "ms");
                    writeLine("time per ping: " + (total / repetitions) + "ms");
                    setState(State.Idle);
                });
        });
}

//
// Run button event handler.
//
$("#run").click(() =>
    {
        //
        // Run the latency loop if not already running.
        //
        if(state !== State.Running)
        {
            setState(State.Running);

            Ice.Promise.try(() => 
                {
                    return run();
                }
            ).catch(ex => 
                {
                    $("#output").val(ex.toString());
                }
            ).finally(() => 
                {
                    setState(State.Idle);
                });
        }
        return false;
    });

//
// Asynchronous loop: each call to the given function returns a
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

//
// Helper function to write the output.
//
function writeLine(msg)
{
    $("#output").val($("#output").val() + msg + "\n");
    $("#output").scrollTop($("#output").get(0).scrollHeight);
}

//
// Handle the client state.
//
const State =
{
    Idle:0,
    Running: 1
};

let state;

function setState(s, ex)
{
    if(s != state)
    {
        switch(s)
        {
            case State.Running:
            {
                $("#output").val("");
                $("#run").addClass("disabled");
                $("#progress").show();
                $("body").addClass("waiting");
                break;
            }
            case State.Idle:
            {
                $("#run").removeClass("disabled");
                $("#progress").hide();
                $("body").removeClass("waiting");
                break;
            }
        }
        state = s;
    }
}

setState(State.Idle);

}());
