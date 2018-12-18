// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <objc/Glacier2.h>
#import <Library.h>
#import <Session.h>
#import <Glacier2Session.h>
#import <Parser.h>
#import <stdio.h>
#import <string.h>

int
runParser(int argc, char* argv[], id<ICECommunicator> communicator)
{
    id<GLACIER2RouterPrx> router = [GLACIER2RouterPrx uncheckedCast:[communicator getDefaultRouter]];
    id<DemoLibraryPrx> library;
    id session;
    if(router != nil)
    {
        printf("This demo accepts any user-id / password combination.\n");

        while(true)
        {
            char id[1024];
            printf("user id: ");
            fflush(stdout);
            fgets(id, sizeof(id), stdin);
            int len = strlen(id);
            if(id[len-1] == '\n')
            {
                id[len-1] = '\0';
            }
            printf("password: ");
            fflush(stdout);
            char pw[1024];
            fgets(pw, sizeof(pw), stdin);
            len = strlen(pw);
            if(pw[len-1] == '\n')
            {
                pw[len-1] = '\0';
            }

            @try
            {
                session = [DemoGlacier2SessionPrx uncheckedCast:
                            [router createSession:[NSString stringWithCString:id encoding:NSUTF8StringEncoding]
                            password:[NSString stringWithCString:pw encoding:NSUTF8StringEncoding]]];
                break;
            }
            @catch(GLACIER2PermissionDeniedException* ex)
            {
                printf("permission denied:\n%s\n", [ex.reason UTF8String]);
            }
            @catch(GLACIER2CannotCreateSessionException* ex)
            {
                printf("cannot create session:\n%s\n", [ex.reason UTF8String]);
            }
        }
        library = [session getLibrary];

        ICELong acmTimeout = [router getACMTimeout];
        if(acmTimeout > 0)
        {
            //
            // Configure the connection to send heartbeats in order to keep our session alive
            //
            [[router ice_getCachedConnection] setACM:@(acmTimeout) close:ICENone heartbeat:@(ICEHeartbeatAlways)];
        }
    }
    else
    {
        id<DemoSessionFactoryPrx> factory = [DemoSessionFactoryPrx checkedCast:[
                communicator propertyToProxy:@"SessionFactory.Proxy"] ];
        if(factory == nil)
        {
            fprintf(stderr, "%s: invalid object reference", argv[0]);
            return 1;
        }

        session = [factory create];
        library = [session getLibrary];
    }

    int rc = [[Parser parserWithLibrary:library] parse];

    if(router)
    {
        @try
        {
            [router destroySession];
        }
        @catch(GLACIER2SessionNotExistException* ex)
        {
            NSLog(@"%@\n", [ex description]);
        }
        @catch(ICEConnectionLostException* ex)
        {
            //
            // Expected: the router closed the connection.
            //
        }
    }
    else
    {
        [session destroy];
    }

    return rc;
}
