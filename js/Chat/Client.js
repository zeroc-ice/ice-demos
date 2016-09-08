// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

(function(){

const RouterPrx = Glacier2.RouterPrx;
const ChatRoomCallbackPrx = Chat.ChatRoomCallbackPrx;
const ChatSessionPrx = Chat.ChatSessionPrx;

//
// Chat client state
//
const State = {Disconnected: 0, Connecting: 1, Connected: 2};
const maxMessageSize = 1024;

let communicator;
let username;
let state;
let hasError = false;

//
// Servant that implements the ChatCallback interface.
// The message operation just writes the received data
// to the output textarea.
//
class ChatCallbackI extends Chat.ChatRoomCallback
{
    init(users)
    {
        users.forEach(name => userJoined(name));
    }

    send(timestamp, name, message)
    {
        if(name != username)
        {
            writeLine(formatDate(timestamp.toNumber()) + " - <" + name + "> - " + unescapeHtml(message));
        }
    }

    join(timestamp, name)
    {
        writeLine(formatDate(timestamp.toNumber()) + " - <system-message> - " + name + " joined.");
        userJoined(name);
    }

    leave(timestamp, name)
    {
        writeLine(formatDate(timestamp.toNumber()) + " - <system-message> - " + name + " left.");
        userLeft(name);
    }
}

function userJoined(name)
{
    if(name == username)
    {
        $("#users").append("<li id=\"" + name + "\"><b>" + name + "</b></li>");
    }
    else
    {
        $("#users").append("<li id=\"" + name + "\"><a href=\"#\">" + name + "</a></li>");
        $("#users #" + name).click(
            function()
            {
                var s = $("#input").val();
                if(s.length > 0)
                {
                    s += " ";
                }
                s += "@" + name + " ";
                $("#input").val(s);
                $("#input").focus();
                return false;
            });
    }
    $("#users").append("<li class=\"divider\"></li>");
}

function userLeft(name)
{
    $("#users #" + name).off("click");
    $("#users #" + name).next().remove();
    $("#users #" + name).remove();
}

function signin()
{
    assert(state === State.Disconnected);
    setState(State.Connecting).then(() =>
        {
            //
            // Get 'config.json' file from server
            //
            const defered = new Ice.Promise();
            $.getJSON('config.json').success(data =>
                {
                    if('Ice.Default.Router' in data)
                    {
                        defered.resolve(data['Ice.Default.Router']);
                    }
                    else
                    {
                        defered.reject(new Error('Ice.Default.Router not in config file'));
                    }
                }).error(ex => defered.reject(ex));

            return defered;
        })
    .then(routerConfig =>
        {
            //
            // Initialize the communicator with the Ice.Default.Router property
            // provided by 'config.json'
            //
            var id = new Ice.InitializationData();
            id.properties = Ice.createProperties();
            id.properties.setProperty("Ice.Default.Router", routerConfig);
            communicator = Ice.initialize(id);

            //
            // Get a proxy to the Glacier2 router using checkedCast to ensure
            // the Glacier2 server is available.
            //
            return RouterPrx.checkedCast(communicator.getDefaultRouter()).then(router =>
                {
                    //
                    // Create a session with the Glacier2 router.
                    //
                    const username = $("#username").val();
                    const password = $("#password").val();

                    return router.createSession(username, password).then(session => 
                        {
                            return run(router, ChatSessionPrx.uncheckedCast(session))
                        });
                });
        })
    .catch(ex =>
        {
            //
            // Handle any exceptions that occurred during session creation.
            //
            if(ex instanceof Glacier2.PermissionDeniedException)
            {
                setState(State.Disconnected, "permission denied:\n" + ex.reason);
            }
            else if(ex instanceof Glacier2.CannotCreateSessionException)
            {
                setState(State.Disconnected, "cannot create session:\n" + ex.reason);
            }
            else if(ex instanceof Ice.ConnectFailedException)
            {
                setState(State.Disconnected, "connection to server failed");
            }
            else
            {
                setState(State.Disconnected, ex.toString());
            }
        });
};

function run(router, session)
{
    assert(state === State.Connecting);
    //
    // The chat promise is used to wait for the completion of chatting
    // state. The completion could happen because the user signed out,
    // or because there is an exception.
    //
    const chat = new Ice.Promise();
    let completed = false;
    //
    // Get the session timeout and the router client category, then
    // create the client object adapter.
    //
    // Use Ice.Promise.all to wait for the completion of all the
    // calls.
    //
    Ice.Promise.all([
        router.getACMTimeout(),
        router.getCategoryForClient(),
        communicator.createObjectAdapterWithRouter("", router)]
    ).then(values =>
        {
            //
            // The results of each promise are provided in an array.
            //
            const [timeout, category, adapter] = values;

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
                        chat.reject(new Error("Connection lost"));
                    }
                });

            //
            // Create the ChatCallback servant and add it to the
            // ObjectAdapter.
            //
            const callback = ChatRoomCallbackPrx.uncheckedCast(
                adapter.add(new ChatCallbackI(),
                            new Ice.Identity("callback", category)));

            //
            // Set the chat session callback.
            //
            return session.setCallback(callback);
        }
    ).then(() => setState(State.Connected)
    ).then(() =>
        {
            //
            // Process input events in the input textbox until
            // the chat promise is completed.
            //
            $("#input").keypress(e =>
                {
                    if(!completed)
                    {
                        //
                        // When enter key is pressed, we send a new message
                        // using the session's say operation and then reset
                        // the textbox contents.
                        //
                        if(e.which === 13)
                        {
                            var msg = $(e.currentTarget).val();
                            if(msg.length > 0)
                            {
                                $(e.currentTarget).val("");
                                if(msg.length > maxMessageSize)
                                {
                                    writeLine("<system-message> - Message length exceeded, " +
                                              "maximum length is " + maxMessageSize + " characters.");
                                }
                                else
                                {
                                    session.send(msg).then(
                                        timestamp =>
                                        {
                                            writeLine(formatDate(timestamp.toNumber()) + " - <" +
                                                      username + "> - " + msg);
                                        },
                                        ex =>
                                        {
                                            if(ex instanceof Chat.InvalidMessageException)
                                            {
                                                writeLine("<system-message> - " + ex.reason);
                                            }
                                            else
                                            {
                                                chat.reject(ex);
                                            }
                                        });
                                }
                            }
                            return false;
                        }
                    }
                });

            //
            // Exit the chat loop accepting the chat promise.
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
            // Reset the input text box and chat output textarea.
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
    ).then(() => setState(State.Disconnected)
    ).catch(ex =>
        {
            //
            // Handle any exceptions that occurred while running.
            //
            setState(State.Disconnected, ex.toString());
        });
};

//
// Do a transition from "from" screen to "to" screen. Return
// a promise that allows us to wait for the transition to
// complete. If to screen is undefined just animate out the
// from screen.
//
function transition(from, to)
{
    var p = new Ice.Promise();

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
// Set default height of output textarea
//
$("#output").height(300);

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
                w = w >= 100 ? 0 : w + 1;
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

//
// Dismiss error message on click.
//
function dismissError()
{
    transition("#signin-alert");
    hasError = false;
    return false;
}

//
// Switch the state and return a promise that is fulfilled
// when state change completes.
//
function setState(newState, error)
{
    assert(state !== newState);
    switch(newState)
    {
        case State.Disconnected:
        {
            assert(state === undefined ||
                   state === State.Connecting ||
                   state === State.Connected);

            $("#users a").off("click");
            $("#users").html("");
            $(window).off("beforeunload");

            //
            // First destroy the communicator if needed then do
            // the screen transition.
            //
            return Ice.Promise.try(() =>
                {
                    if(communicator)
                    {
                        var c = communicator;
                        communicator = null;
                        return c.destroy();
                    }
                }
            ).finally(() =>
                {
                    if(state !== undefined)
                    {
                        stopProgress(false);
                        if(error)
                        {
                            hasError = true;
                            $("#signin-alert span").text(error);
                            
                            stopProgress(false);
                            return transition(state === State.Connecting ? "#loading" : "#chat-form", "#signin-alert").then(
                                () =>
                                {
                                    $("#loading .meter").css("width", "0%");
                                    $("#signin-form").css("display", "block")
                                        .animo({ animation: "flipInX", keep: true });
                                });
                        }
                        else
                        {
                            $("#loading .meter").css("width", "0%");
                            return transition(state === State.Connecting ? "#loading" : "#chat-form", "#signin-form").then(
                                () =>
                                {
                                    $("#signin-form").css("display", "block")
                                        .animo({ animation: "flipInX", keep: true });
                                });
                        }
                    }
                }
            ).then(() =>
                {
                    $("#username").focus();
                    $("#username").keypress(e =>
                        {
                            //
                            // After enter key is pressed in the username input,
                            // switch focus to password input.
                            //
                            if(e.which === 13)
                            {
                                $("#password").focus();
                                return false;
                            }
                        });

                    $("#password").keypress(e =>
                        {
                            //
                            // After enter key is pressed in the password input,
                            // sign-in.
                            //
                            if(e.which === 13)
                            {
                                signin();
                                return false;
                            }
                        });

                    $("#signin").click(() =>
                        {
                            signin();
                            return false;
                        });

                    state = State.Disconnected;
                });
        }
        case State.Connecting:
        {
            assert(state === State.Disconnected);
            username = formatUsername($("#username").val());

            //
            // Remove the signin form event handlers.
            //
            $("#username").off("keypress");
            $("#password").off("keypress");
            $("#signin").off("click");

            //
            // Dismiss any previous error message.
            //
            if(hasError)
            {
                dismissError();
            }

            //
            // Setup a before unload handler to prevent accidentally navigating
            // away from the page while the user is connected to the chat server.
            //
            $(window).on("beforeunload", () =>
                {
                    return "If you navigate away from this page, the current chat session will be lost.";
                });

            //
            // Transition to loading screen
            //
            return transition("#signin-form", "#loading").then(() =>
                {
                    startProgress();
                    state = State.Connecting;
                });
        }
        case State.Connected:
        {
            //
            // Stop animating the loading progress bar and
            // transition to the chat screen.
            //
            assert(state === State.Connecting);
            stopProgress(true);
            return transition("#loading", "#chat-form").then(() =>
                {
                    $("#loading .meter").css("width", "0%");
                    $("#input").focus();
                    state = State.Connected;
                });
        }
    }
}
//
// Switch to initial state.
//
setState(State.Disconnected);

function formatDate(timestamp)
{
    var d = new Date(0);
    d.setUTCMilliseconds(timestamp);
    return d.toLocaleTimeString().trim();
}

function formatUsername(s)
{
    return s.length < 2 ?
        s.toUpperCase() :
        s.substring(0, 1).toUpperCase() + s.substring(1, s.length).toLowerCase();
}

function writeLine(s)
{
    $("#output").val($("#output").val() + s + "\n");
    $("#output").scrollTop($("#output").get(0).scrollHeight);
}

var entities = [
    {entity: /&quot;/g, value: "\""},
    {entity: /&#39;/g, value: "'"},
    {entity: /&lt;/g, value: "<"},
    {entity: /&gt;/g, value: ">"},
    {entity: /&amp;/g, value: "&"}];

function unescapeHtml(msg)
{
    var e;
    for(var i = 0; i < entities.length; ++i)
    {
        e = entities[i];
        msg = msg.replace(e.entity, e.value);
    }
    return msg;
}

function assert(v)
{
    if(!v)
    {
        throw new Error("Assertion failed");
    }
}

}());
