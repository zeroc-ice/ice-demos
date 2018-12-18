// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import {Ice, Glacier2} from "ice";
import {Chat} from "./generated";

(() =>
{

    const RouterPrx = Glacier2.RouterPrx;
    const ChatRoomCallbackPrx = Chat.ChatRoomCallbackPrx;
    const ChatSessionPrx = Chat.ChatSessionPrx;

    //
    // Chat client state
    //
    enum State{Disconnected, Connecting, Connected}
    const maxMessageSize = 1024;

    let communicator:Ice.Communicator;
    let username:string;
    let state:State;

    //
    // Servant that implements the ChatCallback interface.
    // The message operation just writes the received data
    // to the output textarea.
    //
    class ChatCallbackI extends Chat.ChatRoomCallback
    {
        init(users:string[], current:Ice.Current)
        {
            for(let name of users)
            {
                this.userJoined(name)
            }
        }

        send(timestamp:Ice.Long, name:string, message:string, current:Ice.Current)
        {
            if(name != username)
            {
                writeLine(formatDate(timestamp.toNumber()) + " - <" + name + "> - " + unescapeHtml(message));
            }
        }

        join(timestamp:Ice.Long, name:string, current:Ice.Current)
        {
            writeLine(formatDate(timestamp.toNumber()) + " - <system-message> - " + name + " joined.");
            this.userJoined(name);
        }

        leave(timestamp:Ice.Long, name:string, current:Ice.Current)
        {
            writeLine(formatDate(timestamp.toNumber()) + " - <system-message> - " + name + " left.");
            $("#users #" + name).off("click");
            $("#users #" + name).next().remove();
            $("#users #" + name).remove();
        }

        userJoined(name:string)
        {
            if(name == username)
            {
                $("#users").append(`<li id="${name}"><b>${name}</b></li>`);
            }
            else
            {
                $("#users").append(`<li id="${name}"><a href="#">${name}</a></li>`);
                $("#users #" + name).click(
                    () =>
                        {
                            let s:string = $("#input").val() as string;
                            if(s.length > 0)
                            {
                                s += " ";
                            }
                            s += "@" + name + " ";
                            $("#input").val(s);
                            $("#input").focus();
                        });
            }
            $("#users").append("<li class=\"divider\"></li>");
        }
    }

    async function signin()
    {
        try
        {
            await setState(State.Connecting);

            //
            // Get 'config.json' file from server
            //
            let routerConfig:string;

            const data = await $.getJSON('config.json');
            if('Ice.Default.Router' in data)
            {
                routerConfig = data['Ice.Default.Router'];
            }
            else
            {
                throw new Error('Ice.Default.Router not in config file');
            }

            //
            // Initialize the communicator with the Ice.Default.Router property
            // provided by 'config.json'
            //
            const initData = new Ice.InitializationData();
            initData.properties = Ice.createProperties();
            initData.properties.setProperty("Ice.Default.Router", routerConfig);
            communicator = Ice.initialize(initData);

            //
            // Get a proxy to the Glacier2 router using checkedCast to ensure
            // the Glacier2 server is available.
            //
            const router = await RouterPrx.checkedCast(communicator.getDefaultRouter());

            //
            // Create a session with the Glacier2 router.
            //
            const username = $("#username").val() as string;
            const password = $("#password").val() as string;
            const session = await router.createSession(username, password);
            await run(router, ChatSessionPrx.uncheckedCast(session));
        }
        catch(ex)
        {
            console.log(ex.toString());
            //
            // Handle any exceptions that occurred during session creation.
            //
            if(ex instanceof Glacier2.PermissionDeniedException)
            {
                await setState(State.Disconnected, "permission denied:\n" + ex.reason);
            }
            else if(ex instanceof Glacier2.CannotCreateSessionException)
            {
                await setState(State.Disconnected, "cannot create session:\n" + ex.reason);
            }
            else if(ex instanceof Ice.ConnectFailedException)
            {
                await setState(State.Disconnected, "connection to server failed");
            }
            else
            {
                await setState(State.Disconnected, ex.toString());
            }
        }
    }

    async function run(router:Glacier2.RouterPrx, session:Chat.ChatSessionPrx)
    {

        try
        {
            //
            // Get the session timeout and the router client category, then
            // create the client object adapter.
            //
            // Use Promise.all to wait for the completion of all the
            // calls.
            //
            const [timeout, category, adapter] = await Promise.all(
                [
                    router.getACMTimeout(),
                    router.getCategoryForClient(),
                    communicator.createObjectAdapterWithRouter("", router)
                ]);

            //
            // Use ACM heartbeat to keep session alive.
            //
            const connection = router.ice_getCachedConnection();
            if(timeout > 0)
            {
                connection.setACM(timeout, undefined, Ice.ACMHeartbeat.HeartbeatAlways);
            }

            //
            // Create the ChatCallback servant and add it to the
            // ObjectAdapter.
            //
            const callback = ChatRoomCallbackPrx.uncheckedCast(adapter.add(new ChatCallbackI(),
                                                                           new Ice.Identity("callback", category)));

            //
            // Set the chat session callback.
            //
            await session.setCallback(callback);

            await setState(State.Connected);

            //
            // Process input events in the input textbox until completed.
            //
            await new Promise(
                (resolve, reject) =>
                    {
                        $("#input").keypress(
                            e =>
                                {
                                    //
                                    // When enter key is pressed, we send a new message
                                    // using the session's say operation and then reset
                                    // the textbox contents.
                                    //
                                    if(e.which === 13)
                                    {
                                        (async function()
                                        {
                                            const message = $(e.currentTarget).val() as string;
                                            if(message.length > 0)
                                            {
                                                $(e.currentTarget).val("");
                                                if(message.length > maxMessageSize)
                                                {
                                                    writeLine("<system-message> - Message length exceeded, maximum length is " +
                                                              maxMessageSize + " characters.");
                                                }
                                                else
                                                {
                                                    try
                                                    {
                                                        const timestamp = await session.send(message);
                                                        writeLine(formatDate(timestamp.toNumber()) + " - <" + username + "> - " +
                                                                  message);
                                                    }
                                                    catch(ex)
                                                    {
                                                        if(ex instanceof Chat.InvalidMessageException)
                                                        {
                                                            writeLine("<system-message> - " + ex.reason);
                                                        }
                                                        else
                                                        {
                                                            reject(ex);
                                                        }
                                                    }
                                                }
                                            }
                                        }());
                                        return false;
                                    }
                                });

                        connection.setCloseCallback(() => reject(new Error("Connection lost")));
                        $("#signout").click(() =>
                                            {
                                                connection.setCloseCallback(null);
                                                resolve();
                                            });
                    });
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

            try
            {
                //
                // Destroy the session.
                //
                await router.destroySession();
            }
            catch(ex)
            {
            }
            await setState(State.Disconnected);
        }
    }

    //
    // Do a transition from "from" screen to "to" screen. Return
    // a promise that allows us to wait for the transition to
    // complete. If to screen is undefined just animate out the
    // from screen.
    //
    function transition(from:string, to?:string)
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
    // Set default height of output textarea
    //
    $("#output").height(300);

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
                                       w = w >= 100 ? 0 : w + 1;
                                       $("#loading .meter").css("width", w.toString() + "%");
                                   },
                                   25);
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

    //
    // Switch the state and return a promise that is fulfilled
    // when state change completes.
    //
    async function setState(newState:State, error?:string)
    {
        switch(newState)
        {
            case State.Disconnected:
            {
                $("#users a").off("click");
                $("#users").html("");
                $(window).off("beforeunload");

                //
                // First destroy the communicator if needed then do
                // the screen transition.
                //
                if(communicator)
                {
                    await communicator.destroy();
                }
                communicator = null;
                if(state !== undefined)
                {
                    stopProgress(false);
                    $("#loading .meter").css("width", "0%");
                    await transition(state === State.Connecting ? "#loading" : "#chat-form", "#signin-form");

                    if(error)
                    {
                        $("#signin-alert span").text(error);
                        await transition(null, "#signin-alert");
                    }
                }

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
                                            }
                                        });

                $("#signin").click(() => signin());

                state = State.Disconnected;
                break;
            }
            case State.Connecting:
            {
                username = formatUsername($("#username").val() as string);

                //
                // Remove the signin form event handlers.
                //
                $("#username").off("keypress");
                $("#password").off("keypress");
                $("#signin").off("click");

                //
                // Dismiss any previous error message.
                //
                await transition("#signin-alert");

                //
                // Setup a before unload handler to prevent accidentally navigating
                // away from the page while the user is connected to the chat server.
                //
                $(window).on("beforeunload",
                             () => "If you navigate away from this page, the current chat session will be lost.");

                //
                // Transition to loading screen
                //
                await transition("#signin-form", "#loading");
                startProgress();
                state = State.Connecting;
                break;
            }
            case State.Connected:
            {
                //
                // Stop animating the loading progress bar and
                // transition to the chat screen.
                //
                stopProgress(true);
                await transition("#loading", "#chat-form");
                $("#loading .meter").css("width", "0%");
                $("#input").focus();
                state = State.Connected;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    //
    // Switch to initial state.
    //
    setState(State.Disconnected);

    function formatDate(timestamp:number)
    {
        const d = new Date(0);
        d.setUTCMilliseconds(timestamp);
        return d.toLocaleTimeString().trim();
    }

    function formatUsername(s:string)
    {
        return s.length < 2 ?
            s.toUpperCase() :
            s.substring(0, 1).toUpperCase() + s.substring(1, s.length).toLowerCase();
    }

    function writeLine(s:string)
    {
        $("#output").val($("#output").val() + s + "\n");
        $("#output").scrollTop($("#output").get(0).scrollHeight);
    }

    const entities = [
        {escape: /&quot;/g, value: "\""},
        {escape: /&#39;/g, value: "'"},
        {escape: /&lt;/g, value: "<"},
        {escape: /&gt;/g, value: ">"},
        {escape: /&amp;/g, value: "&"}
    ];

    function unescapeHtml(message:string)
    {
        entities.forEach(entity => message.replace(entity.escape, entity.value));
        return message;
    }
})();
