// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

class StopWatch
{
    StopWatch()
    {
    }

    public void
    start()
    {
        _stopped = false;
        _start = System.currentTimeMillis();
    }

    public long
    stop()
    {
        if(!_stopped)
        {
            _stop = System.currentTimeMillis();
            _stopped = true;
        }
        return _stop - _start;
    }

    private boolean _stopped;
    private long _start;
    private long _stop;
}
