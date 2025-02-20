# Copyright (c) ZeroC, Inc.

import Ice
import asyncio

# Load the Slice file that defines the Greeter interface.
Ice.loadSlice('../slice/Greeter.ice')

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

class Chatbot(VisitorCenter.Greeter):

    def __init__(self, loop):
        self.loop = loop

    def greet(self, name, current):
        return asyncio.run_coroutine_threadsafe(self.slow_greet(name), self.loop)

    async def slow_greet(self, name):
        await asyncio.sleep(1)
        print(f"Dispatching greet request {{ name = '{name}' }}")
        return f"Hello, {name}!"
