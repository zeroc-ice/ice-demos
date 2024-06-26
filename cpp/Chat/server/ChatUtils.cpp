//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Chat.h>
#include <ChatUtils.h>

#include <algorithm>
#include <sstream>

using namespace std;

static const unsigned int maxNameSize = 12;
static const unsigned int minNameSize = 3;
constexpr string_view nameRange = "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr string_view isEmptyTokens = "\t\r\n\f\v ";
static const unsigned int maxMessageSize = 1024;

string
validateName(const string& in)
{
    if (in.size() > maxNameSize || in.size() < minNameSize)
    {
        ostringstream msg;
        msg << "Your name must be between " << minNameSize << " and " << maxNameSize << " characters in length.";
        throw invalid_argument(msg.str());
    }
    if (in.find_last_not_of(nameRange) != string::npos)
    {
        throw invalid_argument("Invalid character in name. Valid characters are letter and digits.");
    }
    string out = in;
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    if (out.begin() != out.end())
    {
        transform(out.begin(), out.begin() + 1, out.begin(), ::toupper);
    }
    return out;
}

string
validateMessage(const string& in)
{
    if (in.size() > maxMessageSize)
    {
        ostringstream os;
        os << "Message length exceeded, maximum length is " << maxMessageSize << " characters.";
        throw Chat::InvalidMessageException(os.str());
    }
    if (in.find_last_not_of(isEmptyTokens) == string::npos)
    {
        throw invalid_argument("Your message is empty and was ignored.");
    }
    // Strip html codes in the message
    string out;
    for (const char c : in)
    {
        switch (c)
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
                out.push_back(c);
                break;
            }
        }
    }
    return out;
}
