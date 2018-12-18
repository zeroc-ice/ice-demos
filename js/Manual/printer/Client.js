// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./generated/Printer").Demo;

(async () =>
{
    let communicator;
    try
    {
        communicator = Ice.initialize();
        const base = communicator.stringToProxy("SimplePrinter:default -p 10000");
        const printer = await Demo.PrinterPrx.checkedCast(base);
        if(printer)
        {
            await printer.printString("Hello World!");
        }
        else
        {
            console.log("Invalid proxy");
        }
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
})();
