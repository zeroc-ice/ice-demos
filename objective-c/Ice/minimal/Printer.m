//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#import <Printer.h>

#include <stdio.h>

@implementation Printer
-(void) sayHello:(ICECurrent*)current
{
    printf("Hello World!\n");
    fflush(stdout);
}
@end
