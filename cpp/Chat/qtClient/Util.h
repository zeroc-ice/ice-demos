// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
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
