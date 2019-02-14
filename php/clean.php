<?php

//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

$demos = array(
    "Chat",
    "Ice/hello",
    "Ice/optional",
    "IceDiscovery/hello",
    "Glacier2/hello");

foreach($demos as $demo)
{
    echo "Clean $demo... ";
    $outputDir = "$demo/generated";
    array_map('unlink', glob("$outputDir/*.php"));
    if(is_dir($outputDir))
    {
        rmdir($outputDir);
    }
    echo "ok\n";
}
