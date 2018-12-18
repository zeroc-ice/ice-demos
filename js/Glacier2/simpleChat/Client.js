// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Glacier2 = require("ice").Glacier2;
const Demo = require("./generated/Chat").Demo;

(async function()
{
    //
    // Servant that implements the ChatCallback interface,
    // the message operation just writes the received data
    // to stdout.
    //
    class ChatCallbackI extends Demo.ChatCallback
    {
        message(data)
        {
            console.log(data);
        }
    }

    async function createSession(router)
    {
        try
        {
            //
            // Get a proxy to the default router and down-cast it to Glacier2.Router
            // interface to ensure Glacier2 server is available.
            //
            console.log("This demo accepts any user-id / password combination.");
            process.stdout.write("user id: ");
            const id = await getline();
            process.stdout.write("password: ");
            const password = await getline();
            const session = await router.createSession(id, password);
            return Demo.ChatSessionPrx.uncheckedCast(session);
        }
        catch(ex)
        {
            if(ex instanceof Glacier2.PermissionDeniedException)
            {
                console.log("permission denied:\n" + ex.reason);
            }
            else if(ex instanceof Glacier2.CannotCreateSessionException)
            {
                console.log("cannot create session:\n" + ex.reason);
            }
            else
            {
                throw ex;
            }
        }
    }

    async function runWithSession(router, session)
    {
        //
        // Get the session timeout, the router client category and
        // create the client object adapter. Using Promise.all we
        // can wait for the completion of all the calls at once.
        //
        const [timeout, category, adapter] = await Promise.all(
            [
                router.getACMTimeout(),
                router.getCategoryForClient(),
                router.ice_getCommunicator().createObjectAdapterWithRouter("", router)
            ]);

        //
        // Use ACM heartbeat to keep session alive.
        //
        const connection = router.ice_getCachedConnection();
        if(timeout > 0)
        {
            connection.setACM(timeout, undefined, Ice.ACMHeartbeat.HeartbeatAlways);
        }

        connection.setCloseCallback(() => console.log("Connection lost"));

        //
        // Create the ChatCallback servant and add it to the ObjectAdapter.
        //
        const callback = Demo.ChatCallbackPrx.uncheckedCast(
            adapter.add(new ChatCallbackI(), new Ice.Identity("callback", category)));

        //
        // Set the chat session callback.
        //
        await session.setCallback(callback);

        //
        // The chat function sequentially reads stdin messages and
        // sends them to the server using the session say method.
        //
        while(true)
        {
            process.stdout.write("==> ");
            const message = await getline();

            if(message == "/quit")
            {
                connection.setCloseCallback(null);
                break;
            }
            else if(message.indexOf("/") === 0)
            {
                console.log("enter /quit to exit.");
            }
            else
            {
                await session.say(message);
            }
        }
    }

    let communicator;
    try
    {
        //
        // Initialize the communicator with Ice.Default.Router property
        // set to the simple chat demo Glacier2 router.
        //
        const initData = new Ice.InitializationData();
        initData.properties = Ice.createProperties();
        initData.properties.setProperty("Ice.Default.Router", "DemoGlacier2/router:tcp -p 4063 -h localhost");
        communicator = Ice.initialize(process.argv, initData);

        const router = await Glacier2.RouterPrx.checkedCast(communicator.getDefaultRouter());
        const session = await createSession(router);
        await runWithSession(router, session);
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
}());
