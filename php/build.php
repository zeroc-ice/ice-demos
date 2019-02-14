<?php

//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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

if(file_exists("$iceHome/cpp"))
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

$slice2php = realpath(PHP_OS == "WINNT" ? "$iceBinDir/slice2php.exe" : "$iceBinDir/slice2php");
$sliceDir = realpath(file_exists("$iceHome/share/ice/slice") ? "$iceHome/share/ice/slice" : "$iceHome/slice");

$demos = array(
    "Chat",
    "Ice/hello",
    "Ice/optional",
    "IceDiscovery/hello",
    "Glacier2/hello");

foreach($demos as $demo)
{
    echo "Building $demo... ";
    $outputDir = "$demo/generated";
    $inputFiles = "$demo/*.ice";
    $command = "\"$slice2php\" -I\"$sliceDir\" -I$demo --output-dir $outputDir $inputFiles";
    if(!is_dir($outputDir))
    {
        mkdir($outputDir);
    }
    system($command, $retval);
    if($retval != 0)
    {
        echo "failed!\n";
	echo "$command\n";
        exit($retval);
    }
    echo "ok\n";
}
