// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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
    callback(num/* , current*/)
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
        // Create an object adapter with no name and no endpoints for receiving callbacks
        // over bidirectional connections.
        //
        const adapter = await communicator.createObjectAdapter("");

        //
        // Register the callback receiver servant with the object adapter and activate
        // the adapter.
        //
        const receiver = Demo.CallbackReceiverPrx.uncheckedCast(adapter.addWithUUID(new CallbackReceiverI()));

        //
        // Associate the object adapter with the bidirectional connection.
        //
        proxy.ice_getCachedConnection().setAdapter(adapter);

        //
        // Provide the proxy of the callback receiver object to the server and wait for
        // shutdown.
        //
        await proxy.addClient(receiver);
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
