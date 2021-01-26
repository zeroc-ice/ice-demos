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
            hello = Demo.HelloPrx.uncheckedCast(communicator.propertyToProxy('Hello.Proxy'))
        except Ice.Exception as ex:
            print("invalid proxy:", ex)
            return 1

        menu()

        c = None
        tasks = []
        while c != 'x':
            try:
                sys.stdout.write("==> ")
                sys.stdout.flush()

                # run blocking IO with default executor
                c = (await asyncio.get_running_loop().run_in_executor(None, sys.stdin.readline)).strip()

                # remove completed tasks from the tasks list
                tasks = [t for t in tasks if not t.done()]

                if c == 'i':
                    # create asyncio future from Ice future
                    await Ice.wrap_future(hello.sayHelloAsync(0))
                elif c == 'd':
                    # create asyncio task from coroutine returned by slowSayHello
                    tasks.append(asyncio.create_task(slowSayHello(hello)))
                elif c == 's':
                    # create asyncio future from Ice future
                    await Ice.wrap_future(hello.shutdownAsync())
                elif c == 'x':
                    # cancel pending tasks and wait for them to complete before to exit
                    if len(tasks) > 0:
                        for t in tasks:
                            t.cancel()
                        await asyncio.wait(tasks)
                elif c == '?':
                    menu()
                else:
                    print(f"unknown command {c!r}")
                    menu()
            except Ice.Exception as ex:
                print(ex)

        return 0


async def slowSayHello(hello):
    try:
        # create asyncio future from Ice future
        await Ice.wrap_future(hello.sayHelloAsync(5000))
        print("slow hello completed")
    except Ice.Exception as ex:
        print("task exception:", ex)
    except RuntimeError as ex:
        print("task runtime error:", ex)
    except asyncio.CancelledError:
        # the task is cancelled on exit
        pass


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
