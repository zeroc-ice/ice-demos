// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import {Ice} from "ice";
import {Filesystem} from "./generated";

(async () =>
{
    //
    // Recursively print the contents of directory "dir" in tree fashion.
    // For files, show the contents of each file. The "depth"
    // parameter is the current nesting level (for indentation).
    //
    async function listRecursive(dir:Filesystem.DirectoryPrx, depth:number)
    {
        const indent = "\t".repeat(depth + 1);
        const contents = await dir.list();

        for(const node of contents)
        {
            const subdir = await Filesystem.DirectoryPrx.checkedCast(node);
            console.log(indent + (await node.name()) + (subdir ? " (directory):" : " (file):"));
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
                    console.log(`${indent}\t${line}`);
                }
            }
        }
    }

    let communicator:Ice.Communicator;
    try
    {
        communicator = Ice.initialize(process.argv);
        if(process.argv.length > 2)
        {
            throw new Error("too many arguments");
        }

        //
        // Create a proxy for the root directory
        //
        const base = communicator.stringToProxy("RootDir:default -h localhost -p 10000");

        //
        // Down-cast the proxy to a directory proxy
        //
        const rootDir = await Filesystem.DirectoryPrx.checkedCast(base);
        if(!rootDir)
        {
            throw new Error("Invalid proxy");
        }

        //
        // Recursively list the contents of the root directory
        //
        console.log("Contents of root directory:");
        await listRecursive(rootDir, 0);
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
