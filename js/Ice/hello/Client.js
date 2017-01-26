// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice
const Demo = require("./Hello").Demo;

function menu()
{
    process.stdout.write(
        "usage:\n" +
            "t: send greeting as twoway\n" +
            "o: send greeting as oneway\n" +
            "O: send greeting as batch oneway\n" +
            "f: flush all batch requests\n" +
            "T: set a timeout\n" +
            "P: set a server delay\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n" +
            "\n");
}

let communicator;
let completed = false;
Ice.Promise.try(() =>
    {
        communicator = Ice.initialize(process.argv);
        let proxy = communicator.stringToProxy("hello:default -p 10000").ice_twoway().ice_secure(false);
        let timeout = -1;
        let delay = 0;

        return Demo.HelloPrx.checkedCast(proxy).then(twoway =>
            {
                let oneway = twoway.ice_oneway();
                let batchOneway = twoway.ice_batchOneway();

                menu();
                process.stdout.write("==> ");
                let loop = new Ice.Promise();
                function processKey(key)
                {
                    if(key == "x")
                    {
                        completed = true;
                        loop.resolve();
                        return;
                    }

                    if(key == "t")
                    {
                        return twoway.sayHello(delay);
                    }
                    else if(key == "o")
                    {
                        return oneway.sayHello(delay);
                    }
                    else if(key == "O")
                    {
                        return batchOneway.sayHello(delay);
                    }
                    else if(key == "f")
                    {
                        return batchOneway.ice_flushBatchRequests();
                    }
                    else if(key == "T")
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
                    else if(key == "P")
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
                    else if(key == "s")
                    {
                        return twoway.shutdown();
                    }
                    else if(key == "?")
                    {
                        process.stdout.write("\n");
                        menu();
                    }
                    else
                    {
                        console.log("unknown command `" + key + "'");
                        process.stdout.write("\n");
                        menu();
                    }
                }

                //
                // Process keys sequentially. We chain the promise objects
                // returned by processKey(). Once we have process all the
                // keys we print the prompt and resume the standard input.
                //
                process.stdin.resume();
                let p = Ice.Promise.resolve();
                process.stdin.on("data", buffer =>
                                 {
                                     process.stdin.pause();
                                     var data = buffer.toString("utf-8").trim().split("");
                                     // Process each key
                                     data.forEach(key =>
                                        {
                                            p = p.then(() => processKey(key)).catch(ex => console.log(ex.toString()));
                                        });

                                     // Once we're done, print the prompt
                                     p.then(() =>
                                        {
                                            if(!completed)
                                            {
                                                process.stdout.write("==> ");
                                                process.stdin.resume();
                                            }
                                        });
                                     data = [];
                                 });

                return loop;
            });
    }
).finally(() =>
    {
        if(communicator)
        {
            return communicator.destroy();
        }
    }
).then(
    () => process.exit(0),
    ex =>
    {
        console.log(ex);
        process.exit(1);
    });
