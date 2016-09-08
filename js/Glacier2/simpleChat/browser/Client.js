// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

(function(){

//
// Servant that implements the ChatCallback interface.
// The message operation just writes the received data
// to the output textarea.
//
class ChatCallbackI extends Demo.ChatCallback
{
    message(data)
    {
        $("#output").val($("#output").val() + data + "\n");
        $("#output").scrollTop($("#output").get(0).scrollHeight);
    }
}

//
// Chat client state
//
const State =
{
    Disconnected: 0,
    Connecting: 1,
    Connected:2
};

let state = State.Disconnected;
let hasError = false;

function signin()
{
    let communicator;
    Ice.Promise.try(() =>
        {
            state = State.Connecting;
            //
            // Dismiss any previous error message.
            //
            if(hasError)
            {
                dismissError();
            }
            //
            // Transition to loading screen
            //
            return transition("#signin-form", "#loading");
        }
    ).then(() =>
        {
            //
            // Start animating the loading progress bar.
            //
            startProgress();

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
            const router = secure ? "DemoGlacier2/router:wss -p 9090 -h " + hostname + " -r /chatwss" :
                                    "DemoGlacier2/router:ws -p 8080 -h " + hostname + " -r /chatws";

            //
            // Initialize the communicator with the Ice.Default.Router property
            // set to the simple chat demo Glacier2 router.
            //
            const initData = new Ice.InitializationData();
            initData.properties = Ice.createProperties();
            initData.properties.setProperty("Ice.Default.Router", router);
            communicator = Ice.initialize(initData);

            //
            // Get a proxy to the Glacier2 router using checkedCast to ensure
            // the Glacier2 server is available.
            //
            return Glacier2.RouterPrx.checkedCast(communicator.getDefaultRouter()).then(
                router =>
                {
                    const username = $("#username").val();
                    const password = $("#password").val();
                    
                    return router.createSession(username, password).then(
                        session =>
                        {
                            return run(communicator, router, Demo.ChatSessionPrx.uncheckedCast(session));
                        });
                });
        }
    ).catch(ex =>
        {
            //
            // Handle any exceptions that occurred during session creation.
            //
            if(ex instanceof Glacier2.PermissionDeniedException)
            {
                error("permission denied:\n" + ex.reason);
            }
            else if(ex instanceof Glacier2.CannotCreateSessionException)
            {
                error("cannot create session:\n" + ex.reason);
            }
            else if(ex instanceof Ice.ConnectFailedException)
            {
                error("connection to server failed");
            }
            else
            {
                error(ex.toString());
            }

            if(communicator)
            {
                communicator.destroy();
            }
        });
};

function run(communicator, router, session)
{
    //
    // The chat promise is used to wait for the completion of chatting
    // state. The completion could happen because the user signed out,
    // or because an exception was raised.
    //
    const chat = new Ice.Promise();
    let completed = false;

    //
    // Get the session timeout and the router client category, and
    // create the client object adapter.
    //
    // Use Ice.Promise.all to wait for the completion of all the
    // calls.
    //
    Ice.Promise.all(
        [router.getACMTimeout(),
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
                        error("Connection lost");
                    }
                });

            //
            // Create the ChatCallback servant and add it to the
            // ObjectAdapter.
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
            // Stop animating the loading progress bar and
            // transition to the chat screen.
            //
            stopProgress(true);
            return transition("#loading", "#chat-form");
        }
    ).then(() =>
        {
            $("#loading .meter").css("width", "0%");
            state = State.Connected;
            $("#input").focus();

            //
            // Process input events in the input textbox until the chat
            // promise is completed.
            //
            $("#input").keypress(e =>
                {
                    if(!completed)
                    {
                        //
                        // When the enter key is pressed, we send a new
                        // message using the session say operation and
                        // reset the textbox contents.
                        //
                        if(e.which === 13)
                        {
                            var msg = $(e.currentTarget).val();
                            $(e.currentTarget).val("");
                            session.say(msg).catch(ex => chat.reject(ex));
                            return false;
                        }
                    }
                });

            //
            // Exit the chat loop by accepting the chat
            // promise.
            //
            $("#signout").click(() =>
                {
                    completed = true;
                    chat.resolve();
                    return false;
                });

            return chat;
        }
    ).finally(() =>
        {
            //
            // Reset the input text box and chat output
            // textarea.
            //
            $("#input").val("");
            $("#input").off("keypress");
            $("#signout").off("click");
            $("#output").val("");

            //
            // Destroy the session.
            //
            return router.destroySession();
        }
    ).then(() =>
        {
            //
            // Destroy the communicator and go back to the
            // disconnected state.
            //
            communicator.destroy().finally(() =>
                {
                    transition("#chat-form", "#signin-form").finally(() =>
                        {
                            $("#username").focus();
                            state = State.Disconnected;
                        });
                });
        }
    ).catch(ex =>
        {
            //
            // Handle any exceptions that occurred while running.
            //
            error(ex);
            communicator.destroy();
        });
};

//
// Switch to Disconnected state and display the error
// message.
//
function error(message)
{
    stopProgress(false);
    hasError = true;
    const current = state === State.Connecting ? "#loading" : "#chat-form";
    $("#signin-alert span").text(message);

    //
    // Transition the screen
    //
    transition(current, "#signin-alert").then(() =>
        {
            $("#loading .meter").css("width", "0%");
            $("#signin-form").css("display", "block").animo({ animation: "flipInX", keep: true });
            state = State.Disconnected;
        });
};

//
// Do a transition from "from" screen to "to" screen, return
// a promise that allows us to wait for the transition
// to complete. If to screen is undefined just animate out the
// from screen.
//
function transition(from, to)
{
    const p = new Ice.Promise();
    $(from).animo({ animation: "flipOutX", keep: true }, () =>
        {
            $(from).css("display", "none");
            if(to)
            {
                $(to).css("display", "block").animo({ animation: "flipInX", keep: true }, () => p.resolve());
            }
            else
            {
                p.resolve();
            }
        });
    return p;
};

//
// Event handler for Sign in button
//
$("#signin").click(() =>
                   {
                       signin();
                       return false;
                   });

//
// Dismiss error message.
//
function dismissError()
{
    transition("#signin-alert");
    hasError = false;
    return false;
}

//
// Animate the loading progress bar.
//
var w = 0;
var progress;

function startProgress()
{
    if(!progress)
    {
        progress = setInterval(() =>
            {
                w = w === 100 ? 0 : w + 5;
                $("#loading .meter").css("width", w.toString() + "%");
            },
            20);
    }
};

function stopProgress(completed)
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
};

$("#username").focus();

}());
