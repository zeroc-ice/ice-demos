// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <Hello.h>
#import <stdio.h>

int run(id<ICECommunicator>);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    ICEregisterIceUDP(YES);
    ICEregisterIceSSL(YES);
    ICEregisterIceWS(YES);
#endif

    int status = 0;
    @autoreleasepool
    {
        id<ICECommunicator> communicator = nil;
        @try
        {
            communicator = [ICEUtil createCommunicator:&argc argv:argv configFile:@"config.client"];

            if(argc > 1)
            {
                NSLog(@"%s: too many arguments", argv[0]);
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }
        @catch(ICELocalException* ex)
        {
            NSLog(@"%@", ex);
            status = 1;
        }

        [communicator destroy];
    }
    return status;
}

void menu(void);

int
run(id<ICECommunicator> communicator)
{
    id<DemoHelloPrx> twoway = [DemoHelloPrx checkedCast:
          [[[[communicator propertyToProxy:@"Hello.Proxy"] ice_twoway] ice_timeout:-1] ice_secure:NO]];
    if(!twoway)
    {
        fprintf(stderr, "invalid proxy\n");
        return 1;
    }
    id<DemoHelloPrx> oneway = [twoway ice_oneway];
    id<DemoHelloPrx> batchOneway = [twoway ice_batchOneway];
    id<DemoHelloPrx> datagram = [twoway ice_datagram];
    id<DemoHelloPrx> batchDatagram = [twoway ice_batchDatagram];

    BOOL secure = NO;
    int timeout = -1;
    int delay = 0;

    menu();

    char c = 0;
    do
    {
        @try
        {
            printf("==> ");
            fflush(stdout);

            do
            {
                c = getchar();
            }
            while(c != EOF && c == '\n');
            if(c == 't')
            {
                [twoway sayHello:delay];
            }
            else if(c == 'o')
            {
                [oneway sayHello:delay];
            }
            else if(c == 'O')
            {
                [batchOneway sayHello:delay];
            }
            else if(c == 'd')
            {
                if(secure)
                {
                    printf("secure datagrams are not supported\n");
                    fflush(stdout);
                }
                else
                {
                    [datagram sayHello:delay];
                }
            }
            else if(c == 'D')
            {
                if(secure)
                {
                    printf("secure datagrams are not supported\n");
                    fflush(stdout);
                }
                else
                {
                    [batchDatagram sayHello:delay];
                }
            }
            else if(c == 'f')
            {
                [batchOneway ice_flushBatchRequests];
                [batchDatagram ice_flushBatchRequests];
            }
            else if(c == 'T')
            {
                if(timeout == -1)
                {
                    timeout = 2000;
                }
                else
                {
                    timeout = -1;
                }

                twoway = [twoway ice_invocationTimeout:timeout];
                oneway = [oneway ice_invocationTimeout:timeout];
                batchOneway = [batchOneway ice_invocationTimeout:timeout];

                if(timeout == -1)
                {
                    printf("timeout is now switched off\n");
                    fflush(stdout);
                }
                else
                {
                    printf("timeout is now set to 2000ms\n");
                    fflush(stdout);
                }
            }
            else if(c == 'P')
            {
                if(delay == 0)
                {
                    delay = 2500;
                }
                else
                {
                    delay = 0;
                }

                if(delay == 0)
                {
                    printf("server delay is now deactivated\n");
                    fflush(stdout);
                }
                else
                {
                    printf("server delay is now set to 2500ms\n");
                    fflush(stdout);
                }
            }
            else if(c == 'S')
            {
                secure = !secure;

                twoway = [twoway ice_secure:secure];
                oneway = [oneway ice_secure:secure];
                batchOneway = [batchOneway ice_secure:secure];
                datagram = [datagram ice_secure:secure];
                batchDatagram = [batchDatagram ice_secure:secure];

                if(secure)
                {
                    printf("secure mode is now on\n");
                    fflush(stdout);
                }
                else
                {
                    printf("secure mode is now off\n");
                    fflush(stdout);
                }
            }
            else if(c == 's')
            {
                [twoway shutdown];
            }
            else if(c == 'x')
            {
                // Nothing to do
            }
            else if(c == '?')
            {
                menu();
            }
            else
            {
                printf("unknown command `%c'\n", c);
                menu();
            }
        }
        @catch(ICEException* ex)
        {
            NSLog(@"%@", ex);
        }
    }
    while(c != EOF && c != 'x');

    return 0;
}

void
menu()
{
    printf("usage:\n"
           "t: send greeting as twoway\n"
           "o: send greeting as oneway\n"
           "O: send greeting as batch oneway\n"
           "d: send greeting as datagram\n"
           "D: send greeting as batch datagram\n"
           "f: flush all batch requests\n"
           "T: set a timeout\n"
           "P: set server delay\n"
           "S: switch secure mode on/off\n"
           "s: shutdown server\n"
           "x: exit\n"
           "?: help\n");
}
