<?php

//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

require_once 'Ice.php';

$iceHome = getenv("ICE_HOME");
if($iceHome == NULL)
{
    if(PHP_OS == "WINNT")
    {
        $iceVersion = Ice\stringVersion();
        $iceHome = "C:\\Program Files\\ZeroC\\Ice-$iceVersion";
    }
    else if(PHP_OS == "Darwin")
    {
        $iceHome = "/usr/local";
    }
    else
    {
        $iceHome = "/usr";
    }
}

$srcDist = file_exists("$iceHome/cpp");

if($srcDist)
{
    if(PHP_OS == "WINNT")
    {
        $cppPlatform = getenv("CPP_PLATFORM");
        $cppConfiguration = getenv("CPP_CONFIGURATION");
        $iceBinDir = "$iceHome/cpp/$cppPlatform/$cppConfiguration/bin";
    }
    else
    {
        $iceBinDir = "$iceHome/cpp/bin";
    }
}
else
{
    $iceBinDir = "$iceHome/bin";
}

$slice2php = PHP_OS == "WINNT" ? realpath("$iceBinDir/slice2php.exe") : realpath("$iceBinDir/slice2php");
$sliceDir = file_exists("$iceHome/share/ice/slice") ? "$iceHome/share/ice/slice" : "$iceHome/slice";

$demos = array(
    "Chat",
    "Ice/hello",
    "Ice/optional",
    "IceDiscovery/hello",
    "Glacier2/hello");

foreach($demos as $demo)
{
    echo "Building $demo... ";
    $outputDir = realpath("$demo/generated");
    $inputFiles = "$demo/*.ice";
    $command = "\"$slice2php\" -I\"$sliceDir\" -I$demo --output-dir $outputDir $inputFiles";
    system($command, $retval);
    if($retval != 0)
    {
        echo "failed!\n";
        exit($retval);
    }
    echo "ok\n";
}
