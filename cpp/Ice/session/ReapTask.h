// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef REAP_TASK_H
#define REAP_TASK_H

#include <IceUtil/IceUtil.h>
#include <SessionI.h>
#include <list>

class ReapTask : public IceUtil::TimerTask, public IceUtil::Monitor<IceUtil::Mutex>
{
public:

    ReapTask();

    virtual void runTimerTask();
    void terminate();
    void add(const Demo::SessionPrx&, const SessionIPtr&);

private:

    const IceUtil::Time _timeout;
    struct SessionProxyPair
    {
        SessionProxyPair(const Demo::SessionPrx& p, const SessionIPtr& s) :
            proxy(p), session(s) { }

        // Required to prevent compiler warnings with MSVC++
        SessionProxyPair& operator=(const SessionProxyPair&);

        const Demo::SessionPrx proxy;
        const SessionIPtr session;
    };
    std::list<SessionProxyPair> _sessions;
};
typedef IceUtil::Handle<ReapTask> ReapTaskPtr;

#endif
