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

//
// Recursively print the contents of directory "dir" in tree fashion.
// For files, show the contents of each file. The "depth"
// parameter is the current nesting level (for indentation).
//
async function listRecursive(dir, depth)
{
    const indent = "\t".repeat(depth + 1);
    const contents = await dir.list();

    for(const node of contents)
    {
        const subdir = await Filesystem.DirectoryPrx.checkedCast(node);
        $("#output").val($("#output").val() + indent + (await node.name()) + (subdir ? " (directory):" : " (file):") + "\n");
        if(subdir)
        {
            await listRecursive(subdir, depth);
        }
        else
        {
            const file = Filesystem.FilePrx.uncheckedCast(node);
            const text = await file.read();
            for(const line of text)
            {
                $("#output").val($("#output").val() + indent + "\t" + line + "\n");
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

})();
