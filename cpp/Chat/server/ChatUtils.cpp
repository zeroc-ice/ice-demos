// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatUtils.h>
#include <Chat.h>

#include <sstream>
#include <algorithm>

using namespace std;

static const unsigned int maxNameSize = 12;
static const unsigned int minNameSize = 3;
static const string nameRange = "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const string isEmptyTokens = "\t\r\n\f\v ";

static const unsigned int maxMessageSize = 1024;

string
validateName(const string& in)
{
    if(in.size() > maxNameSize || in.size() < minNameSize)
    {
        ostringstream msg;
        msg << "Your name must be between " << minNameSize << " and " << maxNameSize << " characters in length.";
        throw msg.str();
    }
    if(in.find_last_not_of(nameRange) != string::npos)
    {
        throw string("Invalid character in name. Valid characters are letter and digits.");
    }
    string out = in;
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    if(out.begin() != out.end())
    {
        transform(out.begin(), out.begin() + 1, out.begin(), ::toupper);
    }
    return out;
}

string
validateMessage(const string& in)
{
    if(in.size() > maxMessageSize)
    {
        Chat::InvalidMessageException ex;
        ostringstream msg;
        msg << "Message length exceeded, maximum length is " << maxMessageSize << " characters.";
        ex.reason = msg.str();
        throw ex;
    }
    if(in.find_last_not_of(isEmptyTokens) == string::npos)
    {
        ostringstream msg;
        msg << "Your message is empty and was ignored.";
        throw msg.str();
    }
    // Strip html codes in the message
    string out;
    for(string::const_iterator it = in.begin(); it != in.end(); ++it)
    {
        switch(*it)
        {
            case '&':
            {
                out.append("&amp;");
                break;
            }

            case '"':
            {
                out.append("&quot;");
                break;
            }

            case '\'':
            {
                out.append("&#39;");
                break;
            }

            case '<':
            {
                out.append("&lt;");
                break;
            }

            case '>':
            {
                out.append("&gt;");
                break;
            }

            case '\r':
            case '\n':
            case '\v':
            case '\f':
            case '\t':
            {
                out.append(" ");
                break;
            }

            default:
            {
                out.push_back(*it);
                break;
            }
        }
    }
    return out;
}
