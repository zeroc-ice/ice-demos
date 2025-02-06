// Copyright (c) ZeroC, Inc.

#include "ChatUtils.h"
#include "Chat.h"

#include <array>

using namespace std;

using HtmlEntity = pair<string_view, string_view>;

constexpr std::array<HtmlEntity, 5> htmlEntities = {
    HtmlEntity("&quot;", "\""),
    HtmlEntity("&#39;", "'"),
    HtmlEntity("&lt;", "<"),
    HtmlEntity("&gt;", ">"),
    HtmlEntity("&amp;", "&")};

string
ChatUtils::unstripHtml(const string& s)
{
    string out = s;
    for (const HtmlEntity& entity : htmlEntities)
    {
        for (string::size_type pos = out.find(entity.first); pos != string::npos; pos = out.find(entity.first, pos))
        {
            out.replace(pos, entity.first.size(), entity.second);
        }
    }
    return out;
}

string
ChatUtils::trim(const string& s)
{
    static const string delims = "\t\r\n ";
    const string::size_type last = s.find_last_not_of(delims);
    if (last != string::npos)
    {
        return s.substr(s.find_first_not_of(delims), last + 1);
    }
    return s;
}
