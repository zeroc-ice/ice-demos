# Copyright (c) ZeroC, Inc.

import Ice

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter

class Chatbot(VisitorCenter.Greeter):
    def greet(self, name: str, current: Ice.Current) -> str:
        print(f"Dispatching greet request {{ name = '{name}' }}")
        return f"Hello, {name}!"
