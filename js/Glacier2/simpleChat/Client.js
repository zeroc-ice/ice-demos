// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Glacier2 = require("ice").Glacier2;
const Demo = require("./Chat").Demo;

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

let communicator;

//
// Destroy communicator on SIGINT so application
// exit cleanly.
//
process.once("SIGINT", () =>
{
    if(communicator)
    {
        communicator.destroy().finally(() => process.exit(0));
    }
});

Ice.Promise.try(() =>
    {
        //
        // Initialize the communicator with Ice.Default.Router property
        // set to the simple chat demo Glacier2 router.
        //
        const initData = new Ice.InitializationData();
        initData.properties = Ice.createProperties();
        initData.properties.setProperty("Ice.Default.Router", "DemoGlacier2/router:tcp -p 4063 -h localhost");
        communicator = Ice.initialize(process.argv, initData);

        function createSession()
        {
            return Ice.Promise.try(() =>
                {
                    //
                    // Get a proxy to the default rotuer and down-cast it to Glacier2.Router
                    // interface to ensure Glacier2 server is available.
                    //
                    return Glacier2.RouterPrx.checkedCast(communicator.getDefaultRouter()).then(router =>
                        {
                            console.log("This demo accepts any user-id / password combination.");
                            process.stdout.write("user id: ");
                            return getline().then(id =>
                                {
                                    process.stdout.write("password: ");
                                    return getline().then(password => router.createSession(id, password));
                                }).then(
                                    session => runWithSession(router, Demo.ChatSessionPrx.uncheckedCast(session)),
                                    ex =>
                                    {
                                        if(ex instanceof Glacier2.PermissionDeniedException)
                                        {
                                            console.log("permission denied:\n" + ex.reason);
                                            return createSession();
                                        }
                                        else if(ex instanceof Glacier2.CannotCreateSessionException)
                                        {
                                            console.log("cannot create session:\n" + ex.reason);
                                            return createSession();
                                        }
                                        else
                                        {
                                            throw ex;
                                        }
                                    });
                        });
                });
        }

        function runWithSession(router, session)
        {
            const p = new Ice.Promise();
            
            let completed = false;

            //
            // Get the session timeout, the router client category and
            // create the client object adapter.
            //
            // Use Ice.Promise.all to wait for the completion of all the
            // calls.
            //
            Ice.Promise.all([router.getACMTimeout(),
                             router.getCategoryForClient(),
                             communicator.createObjectAdapterWithRouter("", router)]
            ).then(values =>
                {
                    let [timeout, category, adapter] = values;

                    //
                    // Use ACM heartbeat to keep session alive.
                    //
                    const connection = router.ice_getCachedConnection();
                    if(timeout > 0)
                    {
                        connection.setACM(timeout, undefined, Ice.ACMHeartbeat.HeartbeatAlways);
                    }

                    connection.setCloseCallback(() =>
                        {
                            if(!completed)
                            {
                                console.log("Connection lost");
                            }
                        });

                    //
                    // Create the ChatCallback servant and add it to the ObjectAdapter.
                    //
                    const callback = Demo.ChatCallbackPrx.uncheckedCast(
                        adapter.add(new ChatCallbackI(), new Ice.Identity("callback", category)));

                    //
                    // Set the chat session callback.
                    //
                    return session.setCallback(callback);
                }
            ).then(() =>
                {
                    //
                    // The chat function sequantially reads stdin messages
                    // and send it to server using the session say method.
                    //
                    function chat()
                    {
                        process.stdout.write("==> ");
                        return getline().then(msg =>
                            {
                                if(msg == "/quit")
                                {
                                    completed = true;
                                    p.resolve();
                                }
                                else if(msg.indexOf("/") === 0)
                                {
                                    console.log("enter /quit to exit.");
                                }
                                else
                                {
                                    return session.say(msg);
                                }
                            }
                        ).then(() =>
                            {
                                if(!completed)
                                {
                                    return chat();
                                }
                            }
                        ).catch(ex => p.reject(ex));
                    }

                    //
                    // Start the chat loop
                    //
                    return chat();
                }
            ).finally(() => router.destroySession() // Destroy the session.
            ).catch(ex => p.reject(ex));

            return p;
        }
        return createSession();
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
).then(
    () => process.exit(0),
    ex =>
    {
        //
        // Handle any exceptions above.
        //
        console.log(ex);
        process.exit(1);
    });

//
// Asynchonously process stdin lines using a promise
//
function getline()
{
    const p = new Ice.Promise();
    process.stdin.resume();
    process.stdin.once("data", buffer =>
        {
            process.stdin.pause();
            p.resolve(buffer.toString("utf-8").trim());
        });
    return p;
};
