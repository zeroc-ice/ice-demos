// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import {Ice} from "ice";
import {Demo} from "./generated";

(() =>
{

//
// Define a servant class that implements the Demo.CallbackReceiver
// interface.
//
class CallbackReceiverI extends Demo.CallbackReceiver
{
    callback(num:number, current:Ice.Current)
    {
        writeLine("received callback #" + num);
    }
}

//
// Initialize the communicator
//
const communicator = Ice.initialize();

let connection:Ice.Connection;

async function start()
{
    //
    // Create a proxy to the sender object.
    //
    const hostname = document.location.hostname || "127.0.0.1";

    //
    // Down-cast the proxy to the Demo.CallbackSender interface.
    //
    const server = await Demo.CallbackSenderPrx.checkedCast(
        communicator.stringToProxy("sender:ws -p 10002 -h " + hostname));

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
    connection = server.ice_getCachedConnection();
    connection.setAdapter(adapter);

    //
    // Register the client with the bidir server.
    //
    await server.addClient(receiver);
}

function stop()
{
    //
    // Close the connection, the server will unregister the client
    // when it tries to invoke on the bi-dir proxy.
    //
    return connection.close(Ice.ConnectionClose.GracefullyWithWait);
}

//
// Handle client state
//
enum State
{
    Disconnected,
    Connecting,
    Connected,
    Disconnecting
};

//
// Setup button click handlers
//
$("#start").click(() =>
                  {
                      if(isDisconnected())
                      {
                          setState(State.Connecting);
                          (async function()
                          {
                              try
                              {
                                  await start();
                                  setState(State.Connected);
                              }
                              catch(ex)
                              {
                                  $("#output").val(ex.toString());
                                  setState(State.Disconnected);
                              }
                          }());
                      }
                      return false;
                  });

$("#stop").click(() =>
    {
        if(isConnected())
        {
            setState(State.Disconnecting);
            (async function()
            {
                try
                {
                    await stop();
                }
                catch(ex)
                {
                    $("#output").val(ex.toString());
                }
                finally
                {
                    setState(State.Disconnected);
                }
            }());
        }
        return false;
    });

let state:State;

function isConnected()
{
    return state == State.Connected;
}

function isDisconnected()
{
    return state == State.Disconnected;
}

function writeLine(msg:string)
{
    $("#output").val($("#output").val() + msg + "\n");
    $("#output").scrollTop($("#output").get(0).scrollHeight);
}

function setState(s:State)
{
    if(state != s)
    {
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
    }
}

setState(State.Disconnected);

})();
