// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatUtils.h>
#include <Chat.h>

using namespace std;


typedef pair<const string, const string> HtmlEntity;

static const HtmlEntity htmlEntities[] = { HtmlEntity("&quot;", "\""),
                                           HtmlEntity("&#39;", "'"),
                                           HtmlEntity("&lt;", "<"),
                                           HtmlEntity("&gt;", ">"),
                                           HtmlEntity("&amp;", "&")
};

string
ChatUtils::unstripHtml(const string& s)
{
    string out = s;
    for(unsigned int count = 0; count < sizeof(htmlEntities) / sizeof(htmlEntities[0]); ++count)
    {
        for(string::size_type pos = out.find(htmlEntities[count].first);
            pos != string::npos;
            pos = out.find(htmlEntities[count].first, pos))
        {
            out.replace(pos, htmlEntities[count].first.size(), htmlEntities[count].second);
        }
    }
    return out;
}

string
ChatUtils::trim(const string& s)
{
    static const string delims = "\t\r\n ";
    string::size_type last = s.find_last_not_of(delims);
    if(last != string::npos)
    {
        return s.substr(s.find_first_not_of(delims), last+1);
    }
    return s;
}

