// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import {Ice, Glacier2} from "ice";
import {Demo} from "./generated";

(() =>
{

//
// Servant that implements the ChatCallback interface.
// The message operation just writes the received data
// to the output textarea.
//
class ChatCallbackI extends Demo.ChatCallback
{
    message(data:string, current:Ice.Current)
    {
        $("#output").val($("#output").val() + data + "\n");
        $("#output").scrollTop($("#output").get(0).scrollHeight);
    }
}

//
// Chat client state
//
enum State
{
    Disconnected,
    Connecting,
    Connected
}

let state = State.Disconnected;

async function runWithSession(router:Glacier2.RouterPrx, session:Demo.ChatSessionPrx)
{
    try
    {
        //
        // Get the session timeout and the router client category, and
        // create the client object adapter.
        //
        // Use Promise.all to wait for the completion of all the calls.
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

        connection.setCloseCallback(() => error("Connection lost"));

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
        // Stop animating the loading progress bar and transition to the chat screen.
        //
        stopProgress(true);
        await transition("#loading", "#chat-form");

        $("#loading .meter").css("width", "0%");
        state = State.Connected;
        $("#input").focus();

        //
        // Keep processing input events in the input textbox until signout is clicked
        // or there is an error sending a message.
        //
        await new Promise(
            (resolve, reject) =>
                {
                    $("#input").keypress(
                        e =>
                            {
                                //
                                // When the enter key is pressed, we send a new
                                // message using the session say operation and
                                // reset the textbox contents.
                                //
                                if(e.which === 13)
                                {
                                    (async () =>
                                    {
                                        const message = $(e.currentTarget).val() as string;
                                        $(e.currentTarget).val("");
                                        try
                                        {
                                            await session.say(message);
                                        }
                                        catch(ex)
                                        {
                                            reject(ex);
                                        }
                                    })();
                                    return false;
                                }
                            });

                    $("#signout").click(() =>
                                        {
                                            connection.setCloseCallback(null);
                                            resolve();
                                        });
                });
        //
        // Destroy the session.
        //
        await router.destroySession();
    }
    finally
    {
        //
        // Reset the input text box and chat output textarea.
        //
        $("#input").val("");
        $("#input").off("keypress");
        $("#signout").off("click");
        $("#output").val("");

        await transition("#chat-form", "#signin-form");
        $("#username").focus();
        state = State.Disconnected;
    }
}

async function signin()
{
    let communicator;
    try
    {
        state = State.Connecting;
        //
        // Dismiss any previous error message.
        //
        await transition("#signin-alert");

        //
        // Transition to loading screen.
        //
        await transition("#signin-form", "#loading");

        //
        // Start animating the loading progress bar.
        //
        await startProgress();

        const hostname = document.location.hostname || "127.0.0.1";
        //
        // If the demo is accessed vi https, use a secure (WSS) endpoint, otherwise
        // use a non-secure (WS) endpoint.
        //
        // The web server will act as a reverse proxy for WebSocket connections. This
        // facilitates the setup of WSS with self-signed certificates because Firefox
        // and Internet Explorer certificate exceptions are only valid for the same
        // port and host.
        //
        const secure = document.location.protocol.indexOf("https") != -1;
        const proxy = secure ? "DemoGlacier2/router:wss -p 9090 -h " + hostname + " -r /chatwss" :
                "DemoGlacier2/router:ws -p 8080 -h " + hostname + " -r /chatws";

        //
        // Initialize the communicator with the Ice.Default.Router property
        // set to the simple chat demo Glacier2 router.
        //
        const initData = new Ice.InitializationData();
        initData.properties = Ice.createProperties();
        initData.properties.setProperty("Ice.Default.Router", proxy);
        communicator = Ice.initialize(initData);

        //
        // Get a proxy to the Glacier2 router using checkedCast to ensure
        // the Glacier2 server is available.
        //
        const router = await Glacier2.RouterPrx.checkedCast(communicator.getDefaultRouter());
        const username = $("#username").val() as string;
        const password = $("#password").val() as string;
        const session = await router.createSession(username, password);
        await runWithSession(router, Demo.ChatSessionPrx.uncheckedCast(session));
    }
    catch(ex)
    {
        //
        // Handle any exceptions that occurred during session creation.
        //
        if(ex instanceof Glacier2.PermissionDeniedException)
        {
            await error("permission denied:\n" + ex.reason);
        }
        else if(ex instanceof Glacier2.CannotCreateSessionException)
        {
            await error("cannot create session:\n" + ex.reason);
        }
        else if(ex instanceof Ice.ConnectFailedException)
        {
            await error("connection to server failed");
        }
        else
        {
            await error(ex.toString());
        }
    }
    finally
    {
        if(communicator)
        {
            await communicator.destroy();
        }
    }
}

//
// Switch to Disconnected state and display the error message.
//
async function error(message:string)
{
    if(state == State.Connected)
    {
        $("#input").val("");
        $("#input").off("keypress");
        $("#signout").off("click");
        $("#output").val("");

        await transition("#chat-form", "#signin-form");
        $("#username").focus();
    }
    else if(state == State.Connecting)
    {
        stopProgress(false);
        $("#loading .meter").css("width", "0%");
        await transition("#loading", "#signin-form");
    }
    $("#signin-alert span").text(message);
    await transition(null, "#signin-alert");
    state = State.Disconnected;
}

//
// Do a transition from "from" screen to "to" screen, return
// a promise that allows us to wait for the transition
// to complete. If to screen is undefined just animate out the
// from screen.
//
function transition(from:string, to?:string):PromiseLike<void>
{
    return new Promise(
        resolve =>
            {
                if(from)
                {
                    $(from).fadeOut("slow",
                                    () =>
                                    {
                                        $(from).css("display", "none");
                                        if(to)
                                        {
                                            $(to).css("display", "block").fadeIn("slow", () => resolve());
                                        }
                                        else
                                        {
                                            resolve();
                                        }
                                    });
                }
                else
                {
                    $(to).css("display", "block").fadeIn("slow", () => resolve());
                }
            });
}

//
// Event handler for Sign in button
//
$("#signin").click(signin);

//
// Animate the loading progress bar.
//
let w = 0;
let progress:number;

function startProgress()
{
    if(!progress)
    {
        progress = setInterval(() =>
                                {
                                    w = w === 100 ? 0 : w + 5;
                                    $("#loading .meter").css("width", w.toString() + "%");
                                },
                                100);
    }
}

function stopProgress(completed:boolean)
{
    if(progress)
    {
        clearInterval(progress);
        progress = null;
        if(completed)
        {
            $("#loading .meter").css("width", "100%");
        }
    }
}

$("#username").focus();

})();
