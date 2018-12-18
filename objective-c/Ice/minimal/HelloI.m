// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <HelloI.h>

#include <stdio.h>

@implementation HelloI
-(void) sayHello:(ICECurrent*)current
{
    printf("Hello World!\n");
    fflush(stdout);
}
@end
