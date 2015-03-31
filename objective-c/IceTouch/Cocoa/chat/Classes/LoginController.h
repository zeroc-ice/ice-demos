// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <Cocoa/Cocoa.h>

@protocol ICECommunicator;

@interface LoginController : NSWindowController
{
@private
    IBOutlet NSTextField* chatServerField;
    IBOutlet NSTextField* usernameField;
    IBOutlet NSTextField* passwordField;
    IBOutlet NSWindow* advancedSheet;
    IBOutlet NSWindow* connectingSheet;
    IBOutlet NSProgressIndicator* progress;
    id<ICECommunicator> communicator;
}

-(void)doLogin:(id)sender;
-(void)showAdvancedSheet:(id)sender;
-(void)closeAdvancedSheet:(id)sender;

@end
