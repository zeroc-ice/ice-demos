// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
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
    IBOutlet NSButton* sslField;
    IBOutlet NSButton* glacier2Field;
    IBOutlet NSButton* routerField;
    IBOutlet NSTextField* routerServerField;
    IBOutlet NSWindow* advancedSheet;
    IBOutlet NSWindow* connectingSheet;
    IBOutlet NSProgressIndicator* progress;
    id<ICECommunicator> communicator;
}

-(void)login:(id)sender;
-(void)routerChanged:(id)sender;
-(void)showAdvancedSheet:(id)sender;
-(void)closeAdvancedSheet:(id)sender;

@end
