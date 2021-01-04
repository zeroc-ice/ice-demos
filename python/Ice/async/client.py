#!/usr/bin/env python
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

import asyncio
import sys
import Ice

Ice.loadSlice('Hello.ice')
import Demo


async def main():

    # Ice.initialize returns an initialized Ice communicator; the communicator is destroyed once it goes out of scope.
    with Ice.initialize(sys.argv, "config.client") as communicator:

        # The communicator initialization removes all Ice-related arguments from sys.argv
        if len(sys.argv) > 1:
            print(sys.argv[0] + ": too many arguments")
            return 1

        try:
            hello = Demo.HelloPrx.checkedCast(communicator.propertyToProxy('Hello.Proxy'))
        except Ice.Exception as ex:
            print("invalid proxy:", ex)
            return 1

        menu()

        tasks = set()
        done = set()

        while True:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            nextCharTask = asyncio.create_task(getNextChar())
            tasks.add(nextCharTask)
            while nextCharTask not in done:
                done, tasks = await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
                if nextCharTask in done:
                    c = nextCharTask.result()
                    if c == 'i':
                        # create asyncio future from Ice future
                        tasks.add(Ice.wrap_future(hello.sayHelloAsync(0)))
                    elif c == 'd':
                        # create asyncio task from coroutine returned by slowSayHello
                        tasks.add(asyncio.create_task(slowSayHello(hello)))
                    elif c == 's':
                        tasks.add(Ice.wrap_future(hello.shutdownAsync()))
                    elif c == 'x':
                        # cancel all outstanding tasks
                        for t in tasks:
                            t.cancel()
                        return 0
                    elif c == '?':
                        menu()
                    else:
                        print(f"unknown command {c!r}")
                        menu()
                # retrieve exceptions of done tasks, if any
                for t in done:
                    try:
                        t.result()
                    except Ice.Exception as ex:
                        print("task exception:", ex)
                    except RuntimeError as ex:
                        print("task runtime error:", ex)


async def getNextChar():
    # run blocking IO with default executor
    c = await asyncio.get_running_loop().run_in_executor(None, sys.stdin.readline)
    return c.strip()


async def slowSayHello(hello):
    await Ice.wrap_future(hello.sayHelloAsync(5000))
    print("slow hello completed")


def menu():
    print("""
usage:
i: send immediate greeting
d: send delayed greeting
s: shutdown server
x: exit
?: help
""")


sys.exit(asyncio.run(main()))
