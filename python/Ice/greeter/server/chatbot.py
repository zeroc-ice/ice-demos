# Copyright (c) ZeroC, Inc.

import Ice

# Load the Slice file that defines the Greeter interface.
Ice.loadSlice('../slice/Greeter.ice')

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

class Chatbot(VisitorCenter.Greeter):
    def greet(self, name, current):
        print(f"Dispatching greet request {{ name = '{name}' }}")
        return f"Hello, {name}!"
