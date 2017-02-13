// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject
{
    NSWindowController* loginController;
    BOOL chatActive;
}

-(void)login:(id)sender;
-(void)setChatActive:(BOOL)active;

@end
