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
    $files = glob("$demo/*.ice");
    foreach($files as $file)
    {
        $info = pathinfo($file);
        $generated = $info['dirname'] . "/" . $info['filename'] . ".php";
        if(file_exists($generated))
        {
            unlink($generated);
        }
    }
    echo "ok\n";
}
