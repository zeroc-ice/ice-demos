// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms
// described in the CHAT_DEMO_LICENSE file included in this
// distribution.
//
// **********************************************************************

//
// The proxy of the Glacier2 router.
//
var DefaultRouter = "Glacier2/router:tcp -p 4502 -h demo.zeroc.com";

//
// The access policy protocol. If set to Http, Ice for Silverlight
// configures an outgoing socket to use the HTTP access policy 
// protocol rather than the default TCP protocol.
//
var ClientAccessPolicyProtocol = "Http";

String.prototype.trim = function()
{
    return this.replace(/^\s+|\s+$/g,"");
}

//
// Erase all children of a dom element given its id.
//
function clearChilds(id)
{
    var childs = $(id).childElements();
    for(var cont = 0; cont < childs.length; cont++)
    {
        childs[cont].remove();
    }
}

function processInputEvent(event)
{
    if(event.keyCode == 13)
    {
        var msg = $('txtMessage').value.trim();
        if(msg != "")
        {
            send(msg);
        }
    }
}

function clearInputEvent(event)
{
    if(event.keyCode == 13)
    {
        document.getElementById('txtMessage').value = "";
    }
}

var ChatViewProxy = Class.create(
{
    setState:function(state)
    {
        if(state == "Connecting")
        {
            $('loginContainer').hide();
            $('connectingContainer').show();
            return;
        }
        if(state == "Connected")
        {
            $('menu').show();
            warnOnBeforeUnload(true);
            $('loginContainer').hide();
            $('connectingContainer').hide();
            $('conversationView').show();
            $('logoutLink').show();
            statusBar('Online');
            $('txtMessage').focus();
            return;           
        }
        if(state == "Disconnecting")
        {
            $('menu').hide();
            $('logoutLink').hide();
            return;
        }
        if(state == "Disconnected")
        {
            $('menu').hide();
            warnOnBeforeUnload(false);
            $('logoutLink').hide();
            $('loginLink').hide();
            $('conversationView').hide();
            $('connectingContainer').hide();
            $('loginContainer').show();
            
            statusBar('Offline');
            clearChilds('userList');
            $('txtMessage').value = "";
            clearChilds('messageView');
            clearChilds('errorView');
            $('txtUserName').focus();
            return;
        }
    },
    
    connectionLost:function(error)
    {
        warnOnBeforeUnload(false);
        statusBar("Offline");
        Element.hide('logoutLink');
        $('loginLink').show();
        var w = $('txtMessage').style.width;
        $('txtMessage').replace("<div id=\"txtMessage\"></div>");
        $('txtMessage').style.width = w;
        this.addMessage(error);
    },
    
    addUser:function(name)
    {
        var user = new Element('li', {'id': '_' + name}).update(name);
        $('userList').insert(user);
    },
    
    delUser:function(name)
    {
        $('_' + name).remove();
    },
    
    userSay:function(timestamp, name, message)
    {
        this.addMessage("<p>" + timestamp + " - &lt;" + name + "&gt; " + message+ "</p>");
    },
    
    setError:function(error)
    {
        this.setState("Disconnected");
        $('errorView').insert(new Element('div').update(error));
    },
    
    addMessage:function(message)
    {
        $('messageView').insert(message);
        $('messageView').scrollTop = $('messageView').scrollHeight;
    }
});

// Call the login operation using a scriptable Cs method.
function login()
{
    var sl = getSlControl();
    sl.Content.ChatCoordinator.setDefaultRouter(DefaultRouter);
    sl.Content.ChatCoordinator.setClientAccessPolicyProtocol(ClientAccessPolicyProtocol);
    sl.Content.ChatCoordinator.login($('txtUserName').value, $('txtPassword').value);
}

// Call the logout operation using a scriptable Cs method.
function logout()
{
    var sl = getSlControl();
    sl.Content.ChatCoordinator.logout();
}

// Call the send operation using a scriptable Cs method.
function send(message)
{
    var sl = getSlControl();
    sl.Content.ChatCoordinator.send(message);
}

// Get a reference to the silverlight plugin.
function getSlControl()
{
    return document.getElementById("SilverlightControl");
}

function warnOnBeforeUnload(warn)
{
    if(warn == true)
    {
        window.onbeforeunload = function ()
            {
                return "If you navigate away from this page, the current chat session will be lost.";
            }.bind(this);
    }
    else
    {
        window.onbeforeunload = function ()
            {
            }.bind(this);
    }
}

function loginOnEnterPressed(e)
{
    // if event character code is equal to ascii 13 (if enter key)
    if(e.keyCode == 13)
    {
        login();
        return false;
    }
    else
    {
        return true;
    }
}

function passwordFocus(e)
{
    // if event character code is equal to ascii 13 (if enter key)
    if(e.keyCode == 13)
    {
        $('txtPassword').focus();
        return false;
    }
    else
    {
        return true;
    }
}

function focusId(id)
{
    try
    {
        $(id).focus();
    }
    catch(e)
    {
        // Don't report errors if focus cannot be set.
    }
}

function statusBar(msg)
{
    var statusBar = new Element('p', { 'id': 'statusBar'}).update(msg);
    $('statusBar').replace(statusBar);
}
