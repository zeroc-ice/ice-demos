// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <HelloI.h>

#import <Foundation/NSThread.h>

#import <stdio.h>

@implementation HelloI
-(void) sayHello:(int)delay current:(ICECurrent*)c
{
    if(delay != 0)
    {
        [NSThread sleepForTimeInterval:delay / 1000.f];
    }
    printf("Hello World!\n");
    fflush(stdout);
}

-(void) shutdown:(ICECurrent*)c
{
    printf("Shutting down...\n");
    fflush(stdout);
    [[c.adapter getCommunicator] shutdown];
}

@end
