// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

(function(){

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

//
// Recursively print the contents of directory "dir" in tree fashion.
// For files, show the contents of each file. The "depth"
// parameter is the current nesting level (for indentation).
//
async function listRecursive(dir, depth)
{
    let indent = "\t".repeat(++depth);

    let contents = await dir.list();

    for(index in contents)
    {
        node = contents[index];
        let subdir = await Filesystem.DirectoryPrx.checkedCast(node);
        $("#output").val($("#output").val() + indent + (await node.name()) + (subdir? " (directory):" : " (file):") + "\n");
        if(subdir)
        {
            await listRecursive(subdir, depth);
        }
        else
        {
            let file = Filesystem.FilePrx.uncheckedCast(node);
            text = await file.read();
            for(line in text)
            {
                $("#output").val($("#output").val() + indent + "\t" + text[line] + "\n");
            }
        }
    }
}

async function runDemo()
{
    try
    {
        setState(State.Busy);

        //
        // Create a proxy for the filesystem object
        //
        const hostname = document.location.hostname || "localhost";
        const proxy = communicator.stringToProxy(`RootDir:ws -h ${hostname} -p 10000`);

        const rootDir = await Filesystem.DirectoryPrx.checkedCast(proxy);
        await listRecursive(rootDir, 0);
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
            $("#run").removeClass("disabled").click(runDemo);
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
            $("#run").addClass("disabled").off("click");

            //
            // Display the progress indicator and set the wait cursor.
            //
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

//
// Start in the idle state
//
setState(State.Idle);

}());
