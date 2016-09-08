// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

(function(){

//
// Define a servant class that implements the Demo.CallbackReceiver
// interface.
//
class CallbackReceiverI extends Demo.CallbackReceiver
{
    callback(num, current)
    {
        writeLine("received callback #" + num);
    }
}

//
// Initialize the communicator
//
const communicator = Ice.initialize();

let connection;

function start()
{
    //
    // Create a proxy to the sender object.
    //
    const hostname = document.location.hostname || "127.0.0.1";
    const str = communicator.stringToProxy("sender:ws -p 10002 -h " + hostname);

    //
    // Down-cast the proxy to the Demo.CallbackSender interface.
    //
    return Demo.CallbackSenderPrx.checkedCast(str).then(server =>
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
                    // Set the connection adapter and remember the connection.
                    //
                    connection = server.ice_getCachedConnection();
                    connection.setAdapter(adapter);

                    //
                    // Register the client with the bidir server.
                    //
                    return server.addClient(receiver.ice_getIdentity());
                });
        });
}

function stop()
{
    //
    // Close the connection, the server will unregister the client
    // when it tries to invoke on the bi-dir proxy.
    //
    return connection.close(false);
}

//
// Setup button click handlers
//
$("#start").click(() =>
    {
        if(isDisconnected())
        {
            setState(State.Connecting);
            Ice.Promise.try(() => start().then(() => setState(State.Connected))).catch(ex =>
                {
                    $("#output").val(ex.toString());
                    setState(State.Disconnected);
                });
        }
        return false;
    });

$("#stop").click(() =>
    {
        if(isConnected())
        {
            setState(State.Disconnecting);
            Ice.Promise.try(() => stop()).catch(ex => $("#output").val(ex.toString())).finally(
                () =>
                {
                    setState(State.Disconnected);
                });
        }
        return false;
    });

//
// Handle client state
//
const State =
{
    Disconnected: 0,
    Connecting: 1,
    Connected: 2,
    Disconnecting: 3
};

function isConnected()
{
    return state == State.Connected;
}

function isDisconnected()
{
    return state == State.Disconnected;
}

function writeLine(msg)
{
    $("#output").val($("#output").val() + msg + "\n");
    $("#output").scrollTop($("#output").get(0).scrollHeight);
};

let state;

function setState(s)
{
    if(state == s)
    {
        return;
    }
    state = s;
    switch(s)
    {
        case State.Disconnected:
        {
            $("#start").removeClass("disabled");

            $("#progress").hide();
            $("body").removeClass("waiting");
            break;
        }
        case State.Connecting:
        {
            $("#output").val("");
            $("#start").addClass("disabled");

            $("#progress .message").text("Connecting...");
            $("#progress").show();
            $("body").addClass("waiting");
            break;
        }
        case State.Connected:
        {
            $("#stop").removeClass("disabled");

            $("#progress").hide();
            $("body").removeClass("waiting");
            break;
        }
        case State.Disconnecting:
        {
            $("#stop").addClass("disabled");

            $("#progress .message").text("Disconnecting...");
            $("#progress").show();
            $("body").addClass("waiting");
            break;
        }
        default:
        {
            break;
        }
    }
};

setState(State.Disconnected);

}());
