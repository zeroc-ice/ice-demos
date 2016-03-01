<?php

# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

//
// Required for JSON encoding
//
require_once dirname(__FILE__) . '/JSON.php';

//
// Encode the param $data as Json and send it to the client browser.
//
function printJson($data)
{
    $json = new Services_JSON();
    print($json->encode($data));
}

//
// Check that the Ice extension is loaded.
//
if(!extension_loaded("ice"))
{
    printJson("IcePHP extension is not loaded. Revise your IcePHP installation.");
    error_log("IcePHP extension is not loaded. Revise your IcePHP installation.");
    exit(1);
}

$paths = explode(PATH_SEPARATOR, get_include_path());
$paths = array_merge($paths, array("/usr/share/Ice-3.5.0/php",
    "/Users/joe/Development/zeroc/ice/php/lib",
    "C:\\Program Files\\ZeroC\\Ice-3.5.0\\php",
    "C:\\Program Files (x86)\\ZeroC\\Ice-3.5.0\\php",
    "C:\\Ice-3.5.0\\php"));

$iceIncludePath = "";

foreach($paths as $path)
{
    $path .= '/Ice.php';
    if(is_file($path))
    {
        $iceIncludePath = dirname($path);
        break;
    }
}

if($iceIncludePath == "")
{
    printJson("IcePHP includes not found. Revise your IcePHP installation.");
    error_log("IcePHP includes not found. Revise your IcePHP installation.");
    exit(1);
}

$includePath = get_include_path();
if(!in_array($iceIncludePath, explode(PATH_SEPARATOR, get_include_path())))
{
    if($includePath != '')
    {
        $includePath .= PATH_SEPARATOR;
    }
    $includePath .= $iceIncludePath;
    set_include_path($includePath);
}

require_once 'Ice.php';
require_once dirname(__FILE__) . '/SessionI.php';
require_once dirname(__FILE__) . '/PollingChat.php';

$data = new Ice_InitializationData;
$data->properties = Ice_getProperties();
if($data->properties->getProperty("PollingChatSessionFactory") == '')
{
    $data->properties->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
    $data->properties->setProperty("IceSSL.UsePlatformCAs", "1");
    $data->properties->setProperty("IceSSL.CheckCertName", "1");
    $data->properties->setProperty("PollingChatSessionFactory",
                                   "PollingChatSessionFactory:wss -h zeroc.com -p 443 -r /demo-proxy/chat/poll");
}

$communicator = Ice_initialize($data);

//
// Disable session cookie.
//
ini_set("session.use_cookies", false);

//
// Disable transid in urls.
//
ini_set("session.session.use_transid", false);

if(isset($_POST['id']))
{
    session_id($_POST['id']);
}

//
// Disable caching
//
header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Date in the past

//
// Start the session, but do not report any PHP error to the client if it fails.
// There are exploits that use errors to get knowledge of web server backend internals
// and to search for other system vulnerabilities.
//
@session_start();
if(isset($_SESSION)) // Check that the session started OK
{
    try
    {
        //
        // Get the action from the post params.
        //
        $_action = isset($_POST['action']) ? $_POST['action'] : 'none';

        //
        // Create the session PHP object and pass a reference to $communicator.
        //
        $session = new Session($communicator);

        //
        // Process the action.
        //
        switch($_action)
        {
            case 'login':
            {
                $userName = stripslashes(isset($_POST['username']) ? $_POST['username'] : 'nobody');
                $userPassword = stripslashes(isset($_POST['password']) ? $_POST['password'] : 'nobody');
                printJson($session->login($userName, $userPassword));
                break;
            }
            case 'logout':
            {
                printJson($session->logout());
                break;
            }
            case 'send':
            {
                $message = stripslashes(isset($_POST['message']) ? $_POST['message'] : '');
                printJson($session->send($message));
                break;
            }
            case 'getUpdates':
            {
                printJson($session->getUpdates());
                break;
            }
            case 'getInitialUsers':
            {
                printJson($session->getInitialUsers());
                break;
            }
            default:
            {
                printJson("InvalidActionException");
                break;
            }
        }
    }
    catch(Exception $ex)
    {
        if(!isset($ex->jsontype))
        {
            error_log("Exception: " . $ex);
            $ex->jsontype = "Exception";
        }
        printJson($ex);
    }
}
else
{
    $ex = new Exception("Error starting the PHP session");
    $ex->jsontype = "Exception";
    error_log($ex->getMessage());
    printJson($ex);
}
?>
