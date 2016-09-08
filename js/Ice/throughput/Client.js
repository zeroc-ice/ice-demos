// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./Throughput").Demo;

function menu()
{
    process.stdout.write(
        "usage:\n" +
        "\n" +
        "toggle type of data to send:\n" +
        "1: sequence of bytes (default)\n" +
        "2: sequence of strings (\"hello\")\n" +
        "3: sequence of structs with a string (\"hello\") and a double\n" +
        "4: sequence of structs with two ints and a double\n" +
        "\n" +
        "select test to run:\n" +
        "t: Send sequence as twoway\n" +
        "o: Send sequence as oneway\n" +
        "r: Receive sequence\n" +
        "e: Echo (send and receive) sequence\n" +
        "\n" +
        "other commands:\n" +
        "s: shutdown server\n" +
        "x: exit\n" +
        "?: help\n" +
        "\n");
}

//
// Asynchronous loop, each call to the given function returns a
// promise that when fulfilled runs the next iteration.
//
function loop(fn, repetitions)
{
    var i = 0;
    var next = function()
    {
        if(i++ < repetitions)
        {
            return fn.call().then(next);
        }
    };
    return next();
}

//
// Initialize sequences.
//
const byteSeq = new Buffer(Demo.ByteSeqSize);
for(let i = 0; i < Demo.ByteSeqSize; ++i)
{
    byteSeq[i] = 0;
}

const stringSeq = [];
for(let i = 0; i < Demo.StringSeqSize; ++i)
{
    stringSeq[i] = "hello";
}

const structSeq = [];
for(let i = 0; i < Demo.StringDoubleSeqSize; ++i)
{
    structSeq[i] = new Demo.StringDouble("hello", 3.14);
}

const fixedSeq = [];
for(let i = 0; i < Demo.FixedSeqSize; ++i)
{
    fixedSeq[i] = new Demo.Fixed(0, 0, 0);
}

let communicator;
let completed = false;
Ice.Promise.try(() =>
    {
        let currentType = "1";
        const repetitions = 100;

        let seqSize = Demo.ByteSeqSize;
        let seq = byteSeq;
        let wireSize = 1;

        //
        // Initialize the communicator.
        //
        communicator = Ice.initialize(process.argv);

        //
        // Create a proxy to the throughput object and down-cast the 
        // proxy to the Demo.Throughput interface.
        //
        return Demo.ThroughputPrx.checkedCast(communicator.stringToProxy("throughput:default -p 10000")).then(twoway =>
            {
                const oneway = twoway.ice_oneway();
                menu();
                process.stdout.write("==> ");
                const keyLoop = new Ice.Promise();

                function processKey(key)
                {
                    if(key == "x")
                    {
                        completed = true;
                        keyLoop.resolve();
                        return;
                    }

                    let proxy;
                    let operation;

                    if(key == "1" || key == "2" || key == "3" || key == "4")
                    {
                        currentType = key;

                        //
                        // Select the sequence data type to use by this test.
                        //
                        switch(currentType)
                        {
                            case "1":
                            {
                                console.log("using byte sequences");
                                seqSize = Demo.ByteSeqSize;
                                seq = byteSeq;
                                wireSize = 1;
                                break;
                            }

                            case "2":
                            {
                                console.log("using string sequences");
                                seqSize = Demo.StringSeqSize;
                                seq = stringSeq;
                                wireSize = seq[0].length;
                                break;
                            }

                            case "3":
                            {
                                console.log("using variable-length struct sequences");
                                seqSize = Demo.StringDoubleSeqSize;
                                seq = structSeq;
                                wireSize = seq[0].s.length;
                                wireSize += 8; // Size of double on the wire.
                                break;
                            }

                            case "4":
                            {
                                console.log("using fixed-length struct sequences");
                                seqSize = Demo.FixedSeqSize;
                                seq = fixedSeq;
                                wireSize = 16; // Size of two ints and a double on the wire.
                                break;
                            }
                        }
                    }
                    else if(key == "t" || key == "o" || key == "r" || key == "e")
                    {
                        //
                        // Select the proxy and operation to use by this test.
                        //
                        switch(key)
                        {
                            case "t":
                            case "o":
                            {
                                proxy = key == "o" ? oneway : twoway;
                                if(currentType == 1)
                                {
                                    operation = proxy.sendByteSeq;
                                }
                                else if(currentType == 2)
                                {
                                    operation = proxy.sendStringSeq;
                                }
                                else if(currentType == 3)
                                {
                                    operation = proxy.sendStructSeq;
                                }
                                else if(currentType == 4)
                                {
                                    operation = proxy.sendFixedSeq;
                                }
                                process.stdout.write("sending");
                                break;
                            }

                            case "r":
                            {
                                proxy = twoway;
                                if(currentType == 1)
                                {
                                    operation = proxy.recvByteSeq;
                                }
                                else if(currentType == 2)
                                {
                                    operation = proxy.recvStringSeq;
                                }
                                else if(currentType == 3)
                                {
                                    operation = proxy.recvStructSeq;
                                }
                                else if(currentType == 4)
                                {
                                    operation = proxy.recvFixedSeq;
                                }
                                process.stdout.write("receiving");
                                break;
                            }

                            case "e":
                            {
                                proxy = twoway;
                                if(currentType == 1)
                                {
                                    operation = proxy.echoByteSeq;
                                }
                                else if(currentType == 2)
                                {
                                    operation = proxy.echoStringSeq;
                                }
                                else if(currentType == 3)
                                {
                                    operation = proxy.echoStructSeq;
                                }
                                else if(currentType == 4)
                                {
                                    operation = proxy.echoFixedSeq;
                                }
                                process.stdout.write("sending and receiving");
                                break;
                            }
                        }

                        process.stdout.write(" " + repetitions);
                        switch(currentType)
                        {
                            case "1":
                            {
                                process.stdout.write(" byte");
                                break;
                            }
                            case "2":
                            {
                                process.stdout.write(" string");
                                break;
                            }
                            case "3":
                            {
                                process.stdout.write(" variable-length struct");
                                break;
                            }

                            case "4":
                            {
                                process.stdout.write(" fixed-length struct");
                                break;
                            }
                        }

                        process.stdout.write(" sequences of size " + seqSize);

                        if(key == "o")
                        {
                            process.stdout.write(" as oneway");
                        }
                        console.log("...");

                        let start = new Date().getTime();
                        let args = key != "r" ? [seq] : [];
                        return loop(() => operation.apply(proxy, args), repetitions).then(() =>
                            {
                                //
                                // Write the results.
                                //
                                let total = new Date().getTime() - start;
                                console.log("time for " + repetitions + " sequences: " + total  + " ms");
                                console.log("time per sequence: " + total / repetitions + " ms");

                                let mbit = repetitions * seqSize * wireSize * 8.0 / total / 1000.0;
                                if(key == "e")
                                {
                                    mbit *= 2;
                                }
                                mbit = Math.round(mbit * 100) / 100;
                                console.log("throughput: " + mbit + " Mbps");
                            });
                    }
                    else if(key == "s")
                    {
                        return twoway.shutdown();
                    }
                    else if(key == "?")
                    {
                        process.stdout.write("\n");
                        menu();
                    }
                    else
                    {
                        console.log("unknown command `" + key + "'");
                        process.stdout.write("\n");
                        menu();
                    }
                }

                //
                // Process keys sequentially. We chain the promise objects
                // returned by processKey(). Once we have process all the
                // keys we print the prompt and resume the standard input.
                //
                process.stdin.resume();
                let p = Ice.Promise.resolve();
                process.stdin.on("data", buffer =>
                    {
                        process.stdin.pause();
                        let data = buffer.toString("utf-8").trim().split("");
                        // Process each key
                        data.forEach(key =>
                        {
                            p = p.then(() => processKey(key)).catch(ex => console.log(ex));
                        });
                        // Once we're done, print the prompt
                        p.then(() =>
                        {
                            if(!completed)
                            {
                                process.stdout.write("==> ");
                                process.stdin.resume();
                            }
                        });
                        data = [];
                    });

                return keyLoop;
            });
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
