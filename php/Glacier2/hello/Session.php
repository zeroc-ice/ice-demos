<HTML>
<HEAD>
<TITLE>Session Demo</TITLE>
</HEAD>

<BODY>

<H1>Session Demo</H1>

<?php

//
// Enable error reporting
//
error_reporting(E_ERROR | E_WARNING | E_PARSE);

require_once 'Ice.php';
require_once 'Glacier2.php';
require_once 'Hello.php';

session_start();

try
{
    //
    // Attempt to retrieve the communicator that was registered in Login.php
    // for the session ID.
    //
    $communicator = Ice\find(session_id());
    if(!isset($_SESSION['authenticated']) || $communicator == null)
    {
        echo "<P><HR><B>\n";
        echo "No active session found. Visit the <a href=\"Login.php\">login</a> page to proceed.\n";
        echo "</B><HR></P>\n";
        exit();
    }

    if(isset($_POST["submitted"]))
    {
        if(isset($_POST["sayHello"]))
        {
            $hello = Demo\HelloPrxHelper::uncheckedCast($communicator->stringToProxy("hello:tcp -p 10000"));
            $hello->sayHello(0);
            echo "<P><HR><I>Success.</I><HR></P>\n";
        }
        elseif(isset($_POST["logout"]))
        {
            try
            {
                $router = Glacier2\RouterPrxHelper::uncheckedCast($communicator->getDefaultRouter());
                $router->destroySession();
            }
            catch(Glacier2\SessionNotExistException $ex)
            {
                //
                // This exception is expected if the session has expired.
                //
            }
            catch(Exception $ex)
            {
                // Ignore.
            }
            unset($_SESSION['authenticated']);
            Ice\unregister(session_id());
            echo "<P><HR><B>\n";
            echo "Session destroyed. Visit the <a href=\"Login.php\">login</a> page to proceed.\n";
            echo "</B><HR></P>\n";
            exit();
        }
    }
}
catch(Ice\Exception $ex)
{
    echo "<P><HR><B>\n";
    echo "<PRE>\n";
    print_r($ex);
    echo "</PRE>\n";
    echo "</P>\n";
    echo "<P>\n";
    echo "Visit the <a href=\"Login.php\">login</a> page to proceed.\n";
    echo "</B><HR></P>\n";
    unset($_SESSION['authenticated']);
    Ice\unregister(session_id());
    exit();
}
?>

    <P>
    <FORM method="POST" action="<?php echo basename($_SERVER["PHP_SELF"]); ?>">
        <P>Click <B>Say Hello</B> to invoke on the server and keep the session alive.
        Click <B>Logout</B> to destroy the session.</P>
        </P>
        <P>
        <INPUT type="hidden" name="submitted" value="yes">
        <INPUT type="submit" name="sayHello" value="Say Hello">
        <INPUT type="submit" name="logout" value="Logout">
    </FORM>
</BODY>
</HTML>
