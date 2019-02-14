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
    array_map('unlink', glob("$demo/generated/*.php"));
    echo "ok\n";
}
