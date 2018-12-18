// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./generated/Hello").Demo;

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

        const proxy = communicator.stringToProxy("hello:default -p 10000").ice_twoway().ice_secure(false);
        let timeout = -1;
        let delay = 0;

        let twoway = await Demo.HelloPrx.checkedCast(proxy);
        let oneway = twoway.ice_oneway();
        let batchOneway = twoway.ice_batchOneway();

        menu();
        let line = null;
        do
        {
            process.stdout.write("==> ");
            for(line of await getline())
            {
                try
                {
                    if(line == "t")
                    {
                        await twoway.sayHello(delay);
                    }
                    else if(line == "o")
                    {
                        await oneway.sayHello(delay);
                    }
                    else if(line == "O")
                    {
                        await batchOneway.sayHello(delay);
                    }
                    else if(line == "f")
                    {
                        await batchOneway.ice_flushBatchRequests();
                    }
                    else if(line == "T")
                    {
                        if(timeout == -1)
                        {
                            timeout = 2000;
                        }
                        else
                        {
                            timeout = -1;
                        }
                        twoway = twoway.ice_invocationTimeout(timeout);
                        oneway = oneway.ice_invocationTimeout(timeout);
                        batchOneway = batchOneway.ice_invocationTimeout(timeout);

                        if(timeout == -1)
                        {
                            console.log("timeout is now switched off");
                        }
                        else
                        {
                            console.log("timeout is now set to 2000ms");
                        }
                    }
                    else if(line == "P")
                    {
                        if(delay === 0)
                        {
                            delay = 2500;
                        }
                        else
                        {
                            delay = 0;
                        }

                        if(delay === 0)
                        {
                            console.log("server delay is now deactivated");
                        }
                        else
                        {
                            console.log("server delay is now set to 2500ms");
                        }
                    }
                    else if(line == "s")
                    {
                        await twoway.shutdown();
                    }
                    else if(line == "x")
                    {
                        break;
                    }
                    else if(line == "?")
                    {
                        menu();
                    }
                    else
                    {
                        console.log("unknown command `" + line + "'");
                        menu();
                    }
                }
                catch(ex)
                {
                    console.log(ex.toString());
                }
            }
        }
        while(line != "x");
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

function menu()
{
    process.stdout.write(
`usage:

   t: send greeting as twoway
   o: send greeting as oneway
   O: send greeting as batch oneway
   f: flush all batch requests
   T: set a timeout
   P: set a server delay
   s: shutdown server
   x: exit
   ?: help
`);
}

//
// Asynchonously process stdin lines using a promise
//
function getline()
{
    return new Promise(resolve =>
                       {
                           process.stdin.resume();
                           process.stdin.once("data", buffer =>
                                              {
                                                  process.stdin.pause();
                                                  resolve(buffer.toString("utf-8").trim());
                                              });
                       });
}
