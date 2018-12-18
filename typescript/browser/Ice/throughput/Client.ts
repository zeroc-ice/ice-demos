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
// Initialize sequences.
//
const byteSeq = new Uint8Array(Demo.ByteSeqSize);
for(let i = 0; i < Demo.ByteSeqSize; ++i)
{
    byteSeq[i] = 0;
}

const stringSeq:Demo.StringSeq = [];
for(let i = 0; i < Demo.StringSeqSize; ++i)
{
    stringSeq[i] = "hello";
}

const structSeq:Demo.StringDoubleSeq = [];
for(let i = 0; i < Demo.StringDoubleSeqSize; ++i)
{
    structSeq[i] = new Demo.StringDouble("hello", 3.14);
}

const fixedSeq:Demo.FixedSeq = [];
for(let i = 0; i < Demo.FixedSeqSize; ++i)
{
    fixedSeq[i] = new Demo.Fixed(0, 0, 0);
}

const communicator = Ice.initialize();

//
// Handle the client state.
//
enum State
{
    Idle,
    Running
};

let state:State;

//
// Run the throughput test.
//
async function run()
{
    //
    // Create a proxy to the throughput object.
    //
    const hostname = document.location.hostname || "127.0.0.1";
    const secure = document.location.protocol.indexOf("https") != -1;
    const ref = secure ?
        `throughput:wss -h ${hostname} -p 9090 -r /demowss` :
        `throughput:ws -h ${hostname} -p 8080 -r /demows`;

    //
    // Down-cast the proxy to the Demo.Throughput interface.
    //
    const twoway = await Demo.ThroughputPrx.checkedCast(communicator.stringToProxy(ref));

    const oneway = twoway.ice_oneway();
    const repetitions = 100;

    let seq;
    let seqSize;
    let wireSize;
    let proxy;
    let operation;

    const data = $("#data").val();
    //
    // Get the sequence data
    //
    if(data == "byte-seq")
    {
        seq = byteSeq;
        seqSize = Demo.ByteSeqSize;
        seq = byteSeq;
        wireSize = 1;
    }
    else if(data == "string-seq")
    {
        seq = stringSeq;
        seqSize = Demo.StringSeqSize;
        seq = stringSeq;
        wireSize = seq[0].length;
    }
    else if(data == "struct-seq")
    {
        seq = structSeq;
        seqSize = Demo.StringDoubleSeqSize;
        seq = structSeq;
        wireSize = seq[0].s.length;
        //
        // Size of double on the wire.
        //
        wireSize += 8;
    }
    else if(data == "fixed-seq")
    {
        seq = fixedSeq;
        seqSize = Demo.FixedSeqSize;
        seq = fixedSeq;
        //
        // Size of two ints and a double on the wire.
        //
        wireSize = 16;
    }

    //
    // Get the proxy and operation
    //
    const test = $("#test").val();
    if(test == "twoway" || test == "oneway")
    {
        proxy = test == "twoway" ? twoway : oneway;
        if(data == "byte-seq")
        {
            operation = proxy.sendByteSeq;
        }
        else if(data == "string-seq")
        {
            operation = proxy.sendStringSeq;
        }
        else if(data == "struct-seq")
        {
            operation = proxy.sendStructSeq;
        }
        else if(data == "fixed-seq")
        {
            operation = proxy.sendFixedSeq;
        }
        write("sending");
    }
    else if(test == "receive")
    {
        proxy = twoway;
        if(data == "byte-seq")
        {
            operation = proxy.recvByteSeq;
        }
        else if(data == "string-seq")
        {
            operation = proxy.recvStringSeq;
        }
        else if(data == "struct-seq")
        {
            operation = proxy.recvStructSeq;
        }
        else if(data == "fixed-seq")
        {
            operation = proxy.recvFixedSeq;
        }
        write("receiving");
    }
    else if(test == "echo")
    {
        proxy = twoway;
        if(data == "byte-seq")
        {
            operation = proxy.echoByteSeq;
        }
        else if(data == "string-seq")
        {
            operation = proxy.echoStringSeq;
        }
        else if(data == "struct-seq")
        {
            operation = proxy.echoStructSeq;
        }
        else if(data == "fixed-seq")
        {
            operation = proxy.echoFixedSeq;
        }
        write("sending and receiving");
    }

    write(" " + repetitions);
    if(data == "byte-seq")
    {
        write(" byte");
    }
    else if(data == "string-seq")
    {
        write(" string");
    }
    else if(data == "struct-seq")
    {
        write(" variable-length struct");
    }
    else if(data == "fixed-seq")
    {
        write(" fixed-length struct");
    }
    write(" sequences of size " + seqSize);
    if(test == "oneway")
    {
        write(" as oneway");
    }
    writeLine("...");

    //
    // Invoke the test operation in a loop with the required
    // arguments.
    //
    // We chain the promises. A test operation is called only
    // once the promise for the previous operation is
    // fulfilled.
    //
    const start = new Date().getTime();
    const args = test != "receive" ? [seq] : [];

    for(let i = 0; i < repetitions; i++)
    {
        await operation.apply(proxy, args);
    }

    //
    // Write the results.
    //
    const total = new Date().getTime() - start;
    writeLine(`time for ${repetitions} sequences: ${total} ms`);
    writeLine(`time per sequence: ${total / repetitions} ms`);

    let mbit = repetitions * seqSize * wireSize * 8.0 / total / 1000.0;
    if(test == "echo")
    {
        mbit *= 2;
    }
    mbit = Math.round(mbit * 100) / 100;
    writeLine(`throughput: ${mbit} Mbps`);
}

$("#run").click(async () =>
                {
                    if(state !== State.Running)
                    {
                        setState(State.Running);
                        //
                        // Run the throughput loop if not already running.
                        //
                        try
                        {
                            await run();
                        }
                        catch(ex)
                        {
                            $("#output").val(ex.toString());
                        }
                        finally
                        {
                            setState(State.Idle);
                        }
                    }
                });

//
// Helper functions to write the output.
//
function write(msg:string)
{
    $("#output").val($("#output").val() + msg);
}

function writeLine(msg:string)
{
    write(msg + "\n");
    $("#output").scrollTop($("#output").get(0).scrollHeight);
}

function setState(s:State)
{
    if(s != state)
    {
        switch(s)
        {
            case State.Running:
            {
                $("#output").val("");
                $("#run").addClass("disabled");
                $("#progress").show();
                $("body").addClass("waiting");
                break;
            }
            case State.Idle:
            {
                $("#run").removeClass("disabled");
                $("#progress").hide();
                $("body").removeClass("waiting");
                break;
            }
            default:
            {
                break;
            }
        }
        state = s;
    }
}

setState(State.Idle);

})();
