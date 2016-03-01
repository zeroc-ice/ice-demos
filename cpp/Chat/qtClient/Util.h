// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_UTIL_H
#define CHAT_UTIL_H

#include <Ice/Ice.h>

#include <QWidget>

#include <string>

namespace Chat
{

//
// Centers a window on the screen.
//
void
centerWidgetOnScreen(QWidget* widget);

std::string
unstripHtml(const std::string&);


std::string
formatTimestamp(Ice::Long);

std::string
formatUsername(const std::string&);

}

#endif
