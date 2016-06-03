// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <Cocoa/Cocoa.h>

@protocol ICECommunicator;

@interface LoginController : NSWindowController
{
@private
    IBOutlet NSTextField* usernameField;
    IBOutlet NSTextField* passwordField;
    IBOutlet NSWindow* connectingSheet;
    IBOutlet NSProgressIndicator* progress;
    id<ICECommunicator> communicator;
}

-(void)doLogin:(id)sender;

@end
