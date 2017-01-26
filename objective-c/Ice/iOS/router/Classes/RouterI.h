// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <Router.h>

@class ClientProxyLocator;
@class ServerProxyLocator;
@protocol RouterDelegate;
@protocol ProxyLocator;

@interface RouterBlobject : ICEBlobject<ICEBlobject>
{
@private
    id<ProxyLocator> locator;
    NSObject<RouterDelegate>* routerDelegate;
    int requests;
    int exceptions;
}

@property NSObject<RouterDelegate>* routerDelegate;;
@property (readonly) int requests;
@property (readonly) int exceptions;

-(id)initWithLocator:(id<ProxyLocator>)del;
@end

@interface RouterI : DemoRouter<DemoRouter>
{
@private
    RouterBlobject* clientBlobject;
    RouterBlobject* serverBlobject;

    ClientProxyLocator* clientProxyLocator;
    ServerProxyLocator* serverProxyLocator;
    
    id<ICECommunicator> communicator;
    id<ICEObjectAdapter> routedServerAdapter;
    ICEObjectPrx* serverProxy;
    NSObject<RouterDelegate>* routerDelegate;
    NSString* status;
    id<GLACIER2RouterPrx> router;
    int trace;
}

@property RouterBlobject* clientBlobject;
@property RouterBlobject* serverBlobject;
@property NSString* status;
@property id<GLACIER2RouterPrx> router;
@property (readonly) int clientRequests;
@property (readonly) int clientExceptions;
@property (readonly) int serverRequests;
@property (readonly) int serverExceptions;

-(id)initWithCommunicator:(id<ICECommunicator>)comm;
-(void)setDelegate:(id<RouterDelegate>)delegate;

@end
