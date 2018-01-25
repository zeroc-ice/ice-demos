// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./generated/Callback").Demo;

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

(async function()
{
    let communicator;
    try
    {
        communicator = Ice.initialize(process.argv);
        if(process.argv.length > 2)
        {
            throw new Error("too many arguments");
        }
        //
        // Exit on SIGINT or SIGBREAK
        //
        process.on(process.platform == "win32" ? "SIGBREAK" : "SIGINT", () => communicator.destroy());
        //
        // Create a proxy to the sender object and down-cast the proxy to
        // the Demo.CallbackSender interface.
        //
        const proxy = await Demo.CallbackSenderPrx.checkedCast(communicator.stringToProxy("sender:tcp -p 10000"));
        //
        // Create the client object adapter.
        //
        const adapter = await communicator.createObjectAdapter("");

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
        await proxy.addClient(receiver.ice_getIdentity());

        await communicator.waitForShutdown();
    }
    catch(ex)
    {
        process.exitCode = 1;
        console.log(ex.toString());
        if(communicator)
        {
            await communicator.destroy();
        }
    }
}());
