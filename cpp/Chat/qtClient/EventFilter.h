// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_EVENTFILTER_H
#define CHAT_EVENTFILTER_H

#include <Ice/Ice.h>

#include <QObject>
#include <QEvent>

namespace Chat
{

const int CUSTOM_EVENT_TYPE = QEvent::User + 1000;

class CustomEvent : public QEvent
{
public:

    CustomEvent(const Ice::DispatcherCallPtr&, const Ice::ConnectionPtr&);

    Ice::DispatcherCallPtr
    call() const;

    Ice::ConnectionPtr
    connection() const;

private:

    const Ice::DispatcherCallPtr _call;
    const Ice::ConnectionPtr _connection;
};

class EventFilter : public QObject
{
Q_OBJECT

public:

    EventFilter(QObject*);
    virtual bool eventFilter(QObject* oject, QEvent* event);

signals:

    void dispatchError(const std::string&);
};

}
#endif
