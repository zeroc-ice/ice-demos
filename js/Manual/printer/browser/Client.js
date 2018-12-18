// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

(() =>
{

//
// Handle the client state
//
const State =
{
    Idle: 0,
    Busy: 1
};

//
// Initialize the communicator.
//
const communicator = Ice.initialize();

async function printString()
{
    try
    {
        setState(State.Busy);

        //
        // Create a proxy for the printer object
        //
        const hostname = document.location.hostname || "127.0.0.1";
        const proxy = communicator.stringToProxy(`SimplePrinter:ws -h ${hostname} -p 10000`);

        const printer = await Demo.PrinterPrx.checkedCast(proxy);
        if(printer)
        {
            await printer.printString("Hello World!");
        }
        else
        {
            $("#output").val("Invalid Proxy");
        }
    }
    catch(ex)
    {
        //
        // Handle any exceptions thrown above.
        //
        $("#output").val(ex.toString());
    }
    finally
    {
        setState(State.Idle);
    }
}

function setState(newState)
{
    switch(newState)
    {
        case State.Idle:
        {
            //
            // Hide the progress indicator.
            //
            $("#progress").hide();
            $("body").removeClass("waiting");

            //
            // Enable buttons
            //
            $("#print").removeClass("disabled").click(printString);
            break;
        }
        case State.Busy:
        {
            //
            // Clear any previous error messages.
            //
            $("#output").val("");

            //
            // Disable buttons.
            //
            $("#print").addClass("disabled").off("click");

            //
            // Display the progress indicator and set the wait cursor.
            //
            $("#progress").show();
            $("body").addClass("waiting");
            break;
        }
        default:
        {
            throw new RangeError(`Unexpected state ${newState}`);
        }
    }
}

//
// Start in the idle state
//
setState(State.Idle);

})();
