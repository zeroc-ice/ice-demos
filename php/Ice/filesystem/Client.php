<?php
// Copyright (c) ZeroC, Inc.

require_once "Ice.php";
require_once "Filesystem.php";

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv);

// Create a proxy for the root directory.
$rootDir = Filesystem\DirectoryPrxHelper::createProxy($communicator, "RootDir:tcp -h localhost -p 4061");

// Recursively list the contents of the root directory
echo "Contents of root directory:\n";
listRecursive($rootDir, 0);

// Recursively print the contents of directory "dir" in tree fashion.
// For files, show the contents of each file.
// The "depth" parameter is the current nesting level (for indentation).
function listRecursive($dir, $depth) {
    $depth += 1;
    $indent = str_repeat(" ", $depth);

    $contents = $dir->list();

    foreach ($contents as $node) {
        // The node proxies returned by list() are never null.
        assert($node !== null);

        // Check if this node is a directory by asking the remote object.
        $subdir = Filesystem\DirectoryPrxHelper::checkedCast($node);

        $kind = $subdir !== null ? "(directory)" : "(file)";
        echo $indent . $node->name() . " " . $kind . ":\n";

        if ($subdir !== null) {
            listRecursive($subdir, $depth);
        } else {
            // Read and print the contents of the file.
            $file = Filesystem\FilePrxHelper::uncheckedCast($node);
            $lines = $file->read();
            foreach ($lines as $line) {
                echo $indent . "\t" . $line . "\n";
            }
        }
    }
}
