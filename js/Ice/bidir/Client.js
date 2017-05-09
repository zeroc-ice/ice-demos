// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./Callback").Demo;

//
// Define a servant class that implements Demo.CallbackReceiver
// interface.
//
class CallbackReceiverI extends Demo.CallbackReceiver
{
    callback(num, current)
    {
        console.log("received callback #" + num);
    }
}

let communicator;

//
// Exit on SIGINT or SIGBREAK
//
process.on(process.platform == "win32" ? "SIGBREAK" : "SIGINT", () =>
    {
        if(communicator)
        {
            communicator.destroy().finally(() => process.exit(0));
        }
    });

Ice.Promise.try(() =>
    {
        communicator = Ice.initialize(process.argv);
        if(process.argv.length > 2)
        {
            throw("too many arguments");
        }
        //
        // Initialize the communicator and create a proxy to the sender object.
        //
        const proxy = communicator.stringToProxy("sender:tcp -p 10000");

        //
        // Down-cast the proxy to the Demo.CallbackSender interface.
        //
        return Demo.CallbackSenderPrx.checkedCast(proxy).then(server =>
            {
                //
                // Create the client object adapter.
                //
                return communicator.createObjectAdapter("").then(adapter =>
                    {
                        //
                        // Create a callback receiver servant and add it to
                        // the object adapter.
                        //
                        const receiver = adapter.addWithUUID(new CallbackReceiverI());

                        //
                        // Set the connection adapter.
                        //
                        proxy.ice_getCachedConnection().setAdapter(adapter);

                        //
                        // Register the client with the bidir server.
                        //
                        return server.addClient(receiver.ice_getIdentity());
                    });
            });
    }
).catch(ex =>
    {
        console.log(ex.toString());
        Ice.Promise.try(() =>
            {
                if(communicator)
                {
                    return communicator.destroy();
                }
            }
        ).finally(() => process.exit(1));
    });
