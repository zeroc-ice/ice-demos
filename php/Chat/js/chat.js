// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

var maxMessageSize = 1024;
var coordinator = 0;

String.prototype.trim = function()
{
    return this.replace(/^\s+|\s+$/g,"");
};

function formatDate(timestamp)
{
    date = new Date();
    date.setTime(timestamp);
    var dateStr = date.toLocaleTimeString().trim();
    var i = dateStr.indexOf(' ');
    if(i != -1)
    {
        dateStr = dateStr.substring(0, i);
    }
    return dateStr;
}

function formatUsername(name)
{
    if(name.length <= 2)
    {
        return name;
    }
    return name.substring(0, 1).toUpperCase() + name.substring(1, name.length).toLowerCase();
}

function clearChilds(id)
{
    var childs = $(id).childElements();
    for(var i = 0; i < childs.length; i++)
    {
        childs[i].remove();
    }
}

function stripHtml(input)
{
    var output = "";
    for(var cont = 0; cont < input.length; cont++)
    {
        switch(input.charAt(cont))
        {
            case '&':
            {
                output += "&amp;";
                break;
            }

            case '"':
            {
                output += "&quot;";
                break;
            }

            case '\'':
            {
                output += "&#39;";
                break;
            }

            case '<':
            {
                output += "&lt;";
                break;
            }

            case '>':
            {
                output += "&gt;";
                break;
            }

            case '\r':
            case '\n':
            case '\f':
            case '\t':
            //case '\v': // Ie8 doesn't distigued between 'v' and '\v'
            {
                output += " ";
                break;
            }

            default:
            {
                output += input.charAt(cont);
                break;
            }
        }
    }
    return output;
}

function isNumber(o)
{
    return "number" == typeof o;
}

var Coordinator = Class.create(
{
    initialize:function(chatView)
    {
        this._chatView = chatView;
        this._updater = 0;
        this._init = false;
        this.setConnected(false);
        coordinator = this;
    },

    login:function(name, password)
    {
        var username = name;

        var params = new Hash();
        params.set('action', 'login');
        params.set('username', name);
        params.set('password', password);

        var coordinator = this;
        var opts =
        {
            contentType:'application/x-www-form-urlencoded',
            method:'post',
            encoding:'UTF-8',
            parameters:params,
            onComplete:function(transport)
            {
                Element.hide('connectingContainer');
                Element.show('loginContainer');
                if(transport.status != 200)
                {
                    coordinator.setError("Connection with the web server failed:", "HTTP status code: " +
                                         transport.status);
                    $('txtUserName').focus();
                    return;
                }

                var response = transport.responseText.evalJSON(true);
                if(response.jsontype == 'Session')
                {
                    coordinator.setSessionId(response.id);
                    coordinator.setConnected(true);
                    coordinator._username = formatUsername(username);
                    return;
                }
                if(response.jsontype == "PollingChat_CannotCreateSessionException")
                {
                    coordinator._chatView.appendError("Login failed:", response.reason);
                    $('txtUserName').focus();
                    return;
                }
                if(response.jsontype == "Ice_ConnectionRefusedException")
                {
                    coordinator._chatView.appendError("Communication with server failed (Ice_ConnectionRefusedException) ", "");
                    return;
                }
                if(response.jsontype == "Ice_UnknownLocalException")
                {
                    coordinator._chatView.appendError("Communication with server failed (Ice_UnknownLocalException)",
                                                      response.unknown);
                    $('txtUserName').focus();
                    return;
                }
                coordinator._chatView.appendError("Communication with server failed:", transport.responseText);
                $('txtUserName').focus();
            }
        };
        new Ajax.Request('ChatI.php', opts);
    },

    logout:function()
    {
        if(this.updater)
        {
            this.updater.stop();
            Chat.updater = 0;
        }
        var params = new Hash();
        params.set('id', this._sessionId);
        params.set('action','logout');
        var opts =
        {
            contentType:'application/x-www-form-urlencoded',
            method:'post',
            encoding:'UTF-8',
            parameters:params,
            onComplete: function(transport)
            {
            }
        };
        new Ajax.Request('ChatI.php', opts);
        this.setConnected(false);
    },

    getInitialUsers:function()
    {
        if(coordinator._connected)
        {
            var params = new Hash();
            params.set('id', this._sessionId);
            params.set('action','getInitialUsers');
            var opts =
            {
                contentType:'application/x-www-form-urlencoded',
                method:'post',
                encoding:'UTF-8',
                parameters:params,
                onComplete:function(transport)
                {
                    if(transport.status != 200)
                    {
                        coordinator.connectionLost("<b>Connection with the web server failed: " + transport.status +
                                                   "</b>");
                        return;
                    }
                    var response = transport.responseText.evalJSON(true);
                    if(response.jsontype !== undefined)
                    {
                        if(response.jsontype == "Ice_ConnectionRefusedException")
                        {
                            coordinator.connectionLost("Communication with server failed (Ice_ConnectionRefusedException) ");
                            return;
                        }
                        if(response.jsontype == "Ice_UnknownLocalException")
                        {
                            coordinator.connectionLost("(Ice_UnknownLocalException) " + response.unknown);
                            return;
                        }
                        coordinator.connectionLost(transport.responseText);
                        return;
                    }

                    if(!isNumber(response.length))
                    {
                        coordinator.connectionLost(transport.responseText);
                        return;
                    }

                    for(var i = 0; i < response.length; i++)
                    {
                        var user = new Element('li', { 'id': '_' + response[i] }).update(response[i]);
                        $('userList').insert(user);
                    }
                    coordinator._init = true;
                    coordinator.getUpdates();
                }
            };
            new Ajax.Request('ChatI.php', opts);
        }
    },

    getUpdates:function()
    {
        if(!coordinator._connected || !coordinator._init)
        {
            return;
        }
        var params = new Hash();
        params.set('id', coordinator._sessionId);
        params.set('action','getUpdates');
        var opts =
        {
            contentType:'application/x-www-form-urlencoded',
            method:'post',
            encoding:'UTF-8',
            parameters:params,
            onComplete:function(transport)
            {

                if(!coordinator._connected)
                {
                    return;
                }
                if(transport.status != 200)
                {
                    coordinator.connectionLost("<b>Could not contact web server. HTTP status code: " +
                                               transport.status + "</b>");
                    return;
                }

                var response = transport.responseText.evalJSON(true);

                if(response.jsontype !== undefined)
                {
                    if(response.jsontype == "Ice_ConnectionRefusedException")
                    {
                        coordinator.connectionLost("Communication with server failed (Ice_ConnectionRefusedException) ");
                        return;
                    }
                    if(response.jsontype == "Ice_UnknownLocalException")
                    {
                        coordinator.connectionLost("(Ice_UnknownLocalException) " + response.unknown);
                        return;
                    }

                    coordinator.connectionLost(transport.responseText);
                    return;
                }

                if(!isNumber(response.length))
                {
                    coordinator.connectionLost(transport.responseText);

                    return;
                }

                for(var i = 0; i < response.length; i++)
                {
                    switch(response[i].jsontype)
                    {
                        case "PollingChat_InitialUserEvent":
                            for(var j = 0; j < response[i].users.length; j++)
                            {
                                var user = new Element('li', { 'id': '_' + response[i].users[j] }).update(
                                                       response[i].users[j]);
                                $('userList').insert(user);
                            }
                            break;

                        case "PollingChat_UserJoinedEvent":
                            var user = new Element('li', { 'id': '_' + response[i].name }).update(response[i].name);
                            $('userList').insert(user);
                            coordinator._chatView.addMessage("<p>" + formatDate(response[i].timestamp) +
                                                             " - &lt;system-message&gt; - " + response[i].name +
                                                             " joined.</p>");
                            break;

                        case "PollingChat_UserLeftEvent":
                            $('_' + response[i].name).remove();
                            coordinator._chatView.addMessage("<p>" + formatDate(response[i].timestamp) +
                                                             " - &lt;system-message&gt; - " + response[i].name +
                                                             " left.</p>");
                            break;

                        case "PollingChat_MessageEvent":
                            if(response[i].name != coordinator._username)
                            {
                                coordinator._chatView.addMessage("<p>" + formatDate(response[i].timestamp) +
                                                                 " - &lt;" + response[i].name + "&gt; " +
                                                                 response[i].message + "</p>");
                            }
                            break;

                    }
                }
            }
        };
        new Ajax.Request('ChatI.php', opts);
    },

    send:function(message)
    {

        var params = new Hash();
        var messageSend = message;
        params.set('id', this._sessionId);
        params.set('action','send');
        params.set('message',message);
        var opts =
        {
            contentType:'application/x-www-form-urlencoded',
            method:'post',
            encoding:'UTF-8',
            parameters:params,
            onComplete: function(transport)
            {

                if(transport.status != 200)
                {
                    coordinator.connectionLost("<b>Could not contact web server. HTTP status code: " +
                                               transport.status + "</b>");
                    return;
                }

                var response = eval(transport.responseText);

                if(response.jsontype !== undefined)
                {
                    if(response.jsontype == "Chat_InvalidMessageException")
                    {
                        coordinator._chatView.addMessage("<p>&lt;system-message&gt; - " + response.reason + "</p>");
                        return;
                    }
                    if(response.jsontype == "Ice_ConnectionRefusedException")
                    {
                        coordinator.connectionLost("Communication with server failed (Ice_ConnectionRefusedException) ");
                        return;
                    }
                    if(response.jsontype == "Ice_UnknownLocalException")
                    {
                        coordinator.connectionLost("(Ice_UnknownLocalException) " + response.unknown);
                        return;
                    }
                    coordinator.connectionLost(transport.responseText);
                    return;
                }

                if(isNumber(parseInt(response)))
                {
                    coordinator._chatView.addMessage("<p>" + formatDate(response) + " - &lt;" +
                                                     coordinator._username + "&gt; " + stripHtml(messageSend) +
                                                     "</p>");
                }
            }
        };
        new Ajax.Request('ChatI.php', opts);
    },

    setError:function(error, info)
    {
        this.setConnected(false);
        this._chatView.appendError(error, info);
    },

    connectionLost:function(error)
    {
        if(this._connected)
        {
            warnOnBeforeUnload(false);
            this._connected = false;
            if(this._updater)
            {
                this._init = false;
                this._updater.stop();
                this._updater = 0;
            }
            this._chatView.connectionLost("<p>&lt;system-message&gt;The connection with " +
                                          "the server was unexpectedly lost.<br/>" + error +  "<br/>" +
                                          "<b>You can try to login again from the Login link in the top menu.</b>" +
                                          "</p>");
        }
    },

    setConnected:function(connected)
    {
        this._connected = connected;
        if(connected)
        {
            this.getInitialUsers();
            this._updater = new PeriodicalExecuter(this.getUpdates, 3);
        }
        else
        {
            if(this._updater)
            {
                this._init = false;
                this._updater.stop();
                this._updater = 0;
            }
        }
        this._chatView.setConnected(connected);
    },

    setSessionId:function(sessionId)
    {
        this._sessionId = sessionId;
    }
});

var ChatView = Class.create(
{
    initialize:function()
    {
        this._coordinator = 0;
        this._state = "Offline";
    },

    setCoordinator:function(coordinator)
    {
        this._coordinator = coordinator;
    },

    login:function()
    {
        if(this._state == "Offline")
        {
            this._state = "Connecting";
            this.clearError();
            Element.hide('loginContainer');
            Element.show('connectingContainer');
            this._coordinator.login($('txtUserName').value, $('txtPassword').value);
        }
    },

    logout:function()
    {
        this._coordinator.logout();
        this._state = "Offline";
    },

    connectionLost:function(error)
    {
        warnOnBeforeUnload(false);
        statusBar('Offline');
        Element.hide('logoutLink');
        $('loginLink').show();
        var w = $('txtMessage').style.width;
        $('txtMessage').replace("<div id=\"txtMessage\"></div>");
        $('txtMessage').style.width = w;
        this.addMessage(error);
        this._state = "Offline";
    },

    setConnected:function(connected)
    {
        if(connected)
        {
            warnOnBeforeUnload(true);
            statusBar('Online');
            $('loginContainer').hide();
            $('menu').show();
            $('conversationView').show();
            $('logoutLink').show();
            $('txtMessage').show();
            $('txtMessage').focus();
            this._state = "Online";
        }
        else
        {
            warnOnBeforeUnload(false);
            this.clearConversationView();
            Element.hide('connectingContainer');
            Element.hide('conversationView');
            Element.hide('logoutLink');
            Element.hide('loginLink');
            Element.hide('menu');
            statusBar('Offline');
            $('loginContainer').show();
            $('txtUserName').focus();
            this._state = "Offline";
        }
    },

    clearError:function()
    {
        clearChilds('errorView');
    },

    clearConversationView:function()
    {
        clearChilds('messageView');
        clearChilds('userList');
        $('txtMessage').value = "";
    },

    processInputEvent:function(event)
    {
        if(event.keyCode == 13)
        {
            message = $('txtMessage').value.trim();
            if(message != '')
            {
                if(message.length < maxMessageSize)
                {
                    this._coordinator.send(message);
                }
                else
                {
                    this.addMessage("<p>&lt;system-message&gt; - Message length exceeded, " +
                                    "maximum length is " + maxMessageSize + " characters. </p>");
                }
            }
            return false;
        }
        return true;
    },

    clearInputEvent:function(event)
    {
        if(event.keyCode == 13)
        {
            document.getElementById('txtMessage').value = "";
        }
    },

    appendError:function(name, info)
    {
        $('errorView').insert('<div><b>' + name + '</b></div>');
        $('errorView').insert('<div><i>' + info + '</i></div>');
        this._state = "Offline";
    },

    addMessage:function(message)
    {
        $('messageView').insert(message);
        $('messageView').scrollTop = $('messageView').scrollHeight;
    }
});

var Chat = Class.create(
{
    initialize:function(chatView)
    {
        this._chatView = chatView;
        this._coordinator = new Coordinator(chatView);
        this._chatView.setCoordinator(this._coordinator);
    }
});

function warnOnBeforeUnload(warn)
{
    if(warn == true)
    {
        //
        // Setup the event handler for on before unload event.
        //
        window.onbeforeunload = function ()
            {
                return "If you navigate away from this page, the current chat session will be lost.";
            }.bind(this);
    }
    else
    {
        //
        // Remove the event handler for on before unload event.
        //
        window.onbeforeunload = function ()
            {
            }.bind(this);
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

function loginOnEnterPressed(chatView, e)
{
    // if event character code is equal to ascii 13 (if enter key)
    if(e.keyCode == 13)
    {
        chatView.login();
        return false;
    }
    else
    {
        return true;
    }
}

function statusBar(msg)
{
    var statusBar = new Element('p', { 'id': 'statusBar'}).update(msg);
    $('statusBar').replace(statusBar);
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
