// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <RouterI.h>
#import <RouterDelegate.h>

#pragma mark ProxyLocator

@protocol ProxyLocator<NSObject>

-(id<ICEObjectPrx>)getProxy:(ICECurrent*)current;

@end

#pragma mark RouterBlobject


@implementation RouterBlobject

@synthesize routerDelegate;
@synthesize requests;
@synthesize exceptions;

-(id)initWithLocator:(id<ProxyLocator>)l
{
    if(self = [super init])
    {
        locator = l;
        requests = 0;
        exceptions = 0;
    }
    return self;
}

-(BOOL) ice_invoke:(NSData*)inEncaps outEncaps:(NSMutableData**)outEncaps current:(ICECurrent*)current
{
    id<ICEObjectPrx> proxy = [locator getProxy:current];
    if(proxy == 0)
    {       
        ICEObjectNotExistException* ex = [ICEObjectNotExistException objectNotExistException:__FILE__ line:__LINE__];
        
        //
        // We use a special operation name indicate to the client that
        // the proxy for the Ice object has not been found in our
        // routing table. This can happen if the proxy was evicted
        // from the routing table.
        // 
        ex.id_ = current.id_;
        ex.facet = current.facet;
        ex.operation = @"ice_add_proxy";
        @throw ex;
    }
    
    //
    // Set the correct facet on the proxy.
    //
    if(current.facet.length != 0)
    {
        proxy = [proxy ice_facet:current.facet];
    }

    // If requestId == 0, the request is oneway.
    if(current.requestId == 0)
    {
        proxy = [proxy ice_oneway];
    }
    else //if(current.requestId > 0)
    {
        proxy = [proxy ice_twoway];
    }
    
    @try
    {
        BOOL success = [proxy ice_invoke:current.operation mode:current.mode inEncaps:inEncaps outEncaps:outEncaps];
        @synchronized(self)
        {
            if(success)
            {
                ++requests;
            }
            else
            {
                ++exceptions;
            }

            [routerDelegate performSelectorOnMainThread:@selector(refreshRoutingStatistics) withObject:nil
                                          waitUntilDone:NO];
        }
        return success;
    }
    @catch(ICEException* ex)
    {
        @synchronized(self)
        {
            ++exceptions;
            [routerDelegate performSelectorOnMainThread:@selector(refreshRoutingStatistics) withObject:nil
                                          waitUntilDone:NO];
        }
        @throw ex;
    }
    return NO; // Get rid of warning.
}


@end

#pragma mark ClientProxyLocator

@interface ClientProxyLocator : NSObject<ProxyLocator>
{
@private
    id<ICELogger> logger;
    id<ICERouterPrx> router;

    NSMutableDictionary* routingTable;
    id<ICEConnection> connection;
}

@property id<ICEConnection> connection;
@property id<ICERouterPrx> router;

-(id)initWithLogger:(id<ICELogger>)logger;
-(ICEObjectProxySeq *)add:(ICEMutableObjectProxySeq *)proxies;
-(void)resetRoutingTable:(id<ICEConnection>)connection;

@end

@implementation ClientProxyLocator

@synthesize connection;
@synthesize router;

-(id)initWithLogger:(id<ICELogger>)l
{
    if(self = [super init])
    {
        routingTable = [NSMutableDictionary dictionary];
        logger = l;
    }
    return self;
}

-(ICEObjectProxySeq *) add:(ICEMutableObjectProxySeq *)proxies
{
    @synchronized(self)
    {
        NSEnumerator *e = [proxies objectEnumerator];
        ICEObjectPrx* prx;
        while (prx = [e nextObject])
        {
            if(prx == 0)
            {
                continue;
            }

            if([routingTable objectForKey:prx.ice_getIdentity] != 0)
            {
                continue;
            }

            if(router)
            {
                prx = [prx ice_router:router];
            }
            [routingTable setObject:prx forKey:prx.ice_getIdentity];
        }
    }
    return [ICEObjectProxySeq array];
}

-(void)resetRoutingTable:(id<ICEConnection>)con
{
    @synchronized(self)
    {
        self.connection = con;
        [routingTable removeAllObjects];
    }
}

-(id<ICEObjectPrx>)getProxy:(ICECurrent*)current
{
    @synchronized(self)
    {
        // If the connections are not the same then reject the request.
        if(connection != current.con)
        {
            if(logger)
            {
                [logger trace:@"Router" message:
                 [NSString stringWithFormat:
                  @"rejecting request. no session is associated with the connection.\n"
                  @"identity: category=%@ name=%@.", current.id_.category, current.id_.name]];
            }
            [current.con close:YES];
            @throw [ICEObjectNotExistException objectNotExistException:__FILE__ line:__LINE__];
        }
        return [routingTable objectForKey:current.id_];
    }
    return nil; // Get rid of warning
}

@end

@interface ServerProxyLocator : NSObject<ProxyLocator>
{
@private
    id<ICEConnection> reverseConnection;
    NSString* category;
}

@property id<ICEConnection> reverseConnection;
@property NSString* category;

-(void)resetConnection:(id<ICEConnection>)connection category:(NSString*)category;

@end

#pragma mark ServerProxyLocator

@implementation ServerProxyLocator

@synthesize reverseConnection;
@synthesize category;

-(void)resetConnection:(id<ICEConnection>)connection category:(NSString*)cat
{
    @synchronized(self)
    {
        self.reverseConnection = connection;
        self.category = cat;
    }
}

-(id<ICEObjectPrx>)getProxy:(ICECurrent*)current
{
    @synchronized(self)
    {
        if(![current.id_.category isEqualToString:category])
        {
            @throw [ICEObjectNotExistException objectNotExistException:__FILE__ line:__LINE__];
        }
        return [self.reverseConnection createProxy:current.id_];
    }
    return nil; // Keep the compiler happy.
}

@end

#pragma mark RouterI

@implementation RouterI

@synthesize clientBlobject;
@synthesize serverBlobject;
@synthesize status;
@synthesize router;

-(id)initWithCommunicator:(id<ICECommunicator>)comm
{
    if(self = [super init])
    {
        communicator = comm;
        trace = [[communicator getProperties] getPropertyAsIntWithDefault:@"Trace.Router" value:0];

        id<ICELogger> logger = 0;
        if(trace)
        {
            logger = [communicator getLogger];
        }
        clientProxyLocator = [[ClientProxyLocator alloc] initWithLogger:logger];
        serverProxyLocator = [[ServerProxyLocator alloc] init];
        
        self.clientBlobject = [[RouterBlobject alloc] initWithLocator:clientProxyLocator];
        self.serverBlobject = [[RouterBlobject alloc] initWithLocator:serverProxyLocator];
        
        self.status = @"No connection";
        
        id<ICEObjectAdapter> adapter = [communicator createObjectAdapter:@"Server"];
        [adapter addDefaultServant:serverBlobject category:@""];
        [adapter activate];

        ICEIdentity* ident = [ICEIdentity identity:@"dummy" category:@""];    
        serverProxy = [adapter createProxy:ident];
    }
    return self;
}

-(void)destroySession
{
    // If there is an existing connection to the Glacier2 router, close it.
    if(router)
    {
        @try
        {
            id<ICEConnection> conn = [router ice_getCachedConnection];
            if(conn)
            {
                [conn close:YES];
            }
        }
        @catch(ICELocalException* ex)
        {
            NSLog(@"conn close: %@", ex);
        }
        clientProxyLocator.router = 0;
        self.router = 0;
    }
    
    // Clean up the bi-dir object adapter.
    [routedServerAdapter destroy];
    routedServerAdapter = 0;    
}

// This method is called for both the Router & the RoutedRouter
// createSession calls. Although the signatures are not the same
// it doesn't present an issue due to Objective-C loose typing.
-(NSString*)createSession:(ICECurrent*)c
{
    NSString* category = [ICEUtil generateUUID];

    @synchronized(self)
    {
        [self destroySession];

        // Reset the two proxy locators.
        [clientProxyLocator resetRoutingTable:c.con];
        [serverProxyLocator resetConnection:c.con category:category];

        // Set the status.
        NSString* desc = [c.con toString];
        NSString* pref = @"remote address = ";
        NSUInteger start = [desc rangeOfString:pref].location;
        if(start != NSNotFound)   
        {
            start += pref.length;
            desc = [desc substringWithRange:NSMakeRange(start, desc.length - start)];
        }
        else
        {
            desc = @"unknown";
        }
        self.status = [NSString stringWithFormat:@"Connection from %@", desc];

        [routerDelegate performSelectorOnMainThread:@selector(refreshRoutingStatistics) withObject:nil waitUntilDone:NO];

        if(trace)
        {
            [[communicator getLogger] trace:@"Router" message:
             [NSString stringWithFormat:@"Router::createSession category: %@ con: %@", category, desc]];
        }
    }

    return category;
}

-(void) createGlacier2Session:(id<ICERouterPrx>)r
                        userId:(NSMutableString *)userId
                     password:(NSMutableString *)password
                     category:(NSString **)category
               sessionTimeout:(ICELong*)sessionTimeout
                         sess:(id<GLACIER2SessionPrx> *)sess
                      current:(ICECurrent *)c
{
    @synchronized(self)
    {
        [self destroySession];

        id<GLACIER2RouterPrx> glacier2router = [GLACIER2RouterPrx checkedCast:r];
        *sess = [glacier2router createSession:userId password:password];
        *category = [glacier2router getCategoryForClient];
        *sessionTimeout = [glacier2router getSessionTimeout];
        
        // Reset the two proxy locators.
        [clientProxyLocator resetRoutingTable:c.con];
        [serverProxyLocator resetConnection:c.con category:*category];
        
        // Set the status.
        NSString* desc = [c.con toString];
        NSString* pref = @"remote address = ";
        NSUInteger start = [desc rangeOfString:pref].location;
        if(start != NSNotFound)   
        {
            start += pref.length;
            desc = [desc substringWithRange:NSMakeRange(start, desc.length - start)];
        }
        else
        {
            desc = @"unknown";
        }
        self.status = [NSString stringWithFormat:@"Connection from %@", desc];
        
        serverProxyLocator.category = *category;
        self.router = glacier2router;
        clientProxyLocator.router = glacier2router;
        
        // Create a new bi-dir OA for callbacks.
        NSAssert(routedServerAdapter == 0, @"serverAdapter == 0");
        routedServerAdapter = [communicator createObjectAdapterWithRouter:@"RoutedServer" router:r];
        [routedServerAdapter addDefaultServant:serverBlobject category:*category];
        [routedServerAdapter activate];
        
        [routerDelegate performSelectorOnMainThread:@selector(refreshRoutingStatistics) withObject:nil waitUntilDone:NO];

        if(trace)
        {
            [[communicator getLogger] trace:@"Router" message:
             [NSString stringWithFormat:@"createGlacier2Session: created new session:\nrouter: %@\nuserId: %@\ncategory: %@",
              [r description], userId, *category]];
        }
    }
}

-(void)setDelegate:(id<RouterDelegate>)del
{
    routerDelegate = del;
    clientBlobject.routerDelegate = routerDelegate;
    serverBlobject.routerDelegate = routerDelegate;
}

-(id<ICEObjectPrx>) getClientProxy:(ICECurrent *)current
{
    // Since we don't support pre-Ice 3.2 clients, return a nil proxy.
    return 0;
}

-(id<ICEObjectPrx>) getServerProxy:(ICECurrent *)current
{
    @synchronized(self)
    {
        // If we are routing through Glacier2, we must return the server proxy from Glacier2.
        if(router != 0)
        {
            return [router getServerProxy];
        }
        return serverProxy;
    }
    return 0; // Keep the compiler happy.
}

-(void) addProxy:(id<ICEObjectPrx>)proxy current:(ICECurrent *)current
{
    ICEMutableObjectProxySeq* seq = [ICEMutableObjectProxySeq arrayWithObject:proxy];
    [self addProxies:seq current:current];
}

-(ICEObjectProxySeq *) addProxies:(ICEMutableObjectProxySeq *)proxies current:(ICECurrent *)current
{
    return [clientProxyLocator add:proxies];
}

-(int)clientRequests
{
    return clientBlobject.requests;
}

-(int)clientExceptions
{
    return clientBlobject.exceptions;
}

-(int)serverRequests
{
    return serverBlobject.requests;
}

-(int)serverExceptions
{
    return serverBlobject.exceptions;
}


@end
