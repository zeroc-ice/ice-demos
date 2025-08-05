// Copyright (c) ZeroC, Inc.

import { Ice } from "@zeroc/ice";
import { Filesystem } from "./Filesystem.js";
import process from "node:process";

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using communicator = Ice.initialize(process.argv);

// Create a proxy for the root directory.
const rootDir = new Filesystem.DirectoryPrx(communicator, "RootDir:tcp -h localhost -p 4061");

// Recursively list the contents of the root directory.
console.log("Contents of root directory:");
await listRecursive(rootDir);

/**
 * Recursively print the contents of a directory in tree fashion. For files, show the contents of each file.
 *
 * @param dir The directory to list.
 * @param depth The current nesting level (for indentation).
 **/
async function listRecursive(dir: Filesystem.DirectoryPrx, depth: number = 0) {
    var indent = "\t".repeat(++depth);

    const contents = await dir.list();

    for (const node of contents) {
        const subdir = await Filesystem.DirectoryPrx.checkedCast(node!);
        const kind = subdir != null ? "directory" : "file";
        const name = await node!.name();
        console.log(`${indent}${name} ${kind}:`);

        if (subdir != null) {
            await listRecursive(subdir, depth);
        } else {
            const file: Filesystem.FilePrx = Filesystem.FilePrx.uncheckedCast(node!);
            const lines = await file.read();
            for (const line of lines) {
                console.log(`${indent}\t${line}`);
            }
        }
    }
}
