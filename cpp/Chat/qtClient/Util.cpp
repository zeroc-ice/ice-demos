// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Util.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QDateTime>

using namespace std;

void
Chat::centerWidgetOnScreen(QWidget* widget)
{
    QRect r0 = QApplication::desktop()->screen()->geometry();
    int x = ((r0.width() - widget->frameGeometry().width()) / 2);
    int y = ((r0.height() - widget->frameGeometry().height()) / 2);
    widget->move(QPoint(x, y));
}

namespace
{

typedef pair<const string, const string> HtmlEntity;

const HtmlEntity htmlEntities[] = { HtmlEntity("&quot;", "\""),
                                    HtmlEntity("&#39;", "'"),
                                    HtmlEntity("&lt;", "<"),
                                    HtmlEntity("&gt;", ">"),
                                    HtmlEntity("&amp;", "&")};
}

string
Chat::unstripHtml(const string& s)
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
Chat::formatTimestamp(Ice::Long timestamp)
{
    QDateTime t;
#if QT_VERSION >= 0x040700
    t.setMSecsSinceEpoch(timestamp);
#else
    t =  t.addMSecs(timestamp);
#endif
    return t.toLocalTime().toString("hh:mm:ss").toUtf8().data();
}

string
Chat::formatUsername(const std::string& username)
{
    string out = username;
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    if(out.begin() != out.end())
    {
        transform(out.begin(), out.begin() + 1, out.begin(), ::toupper);
    }
    return out;
}
