# Copyright (c) ZeroC, Inc.

import asyncio

import Ice

# Slice module VisitorCenter in Greeter.ice maps to Python module VisitorCenter.
import VisitorCenter


class Chatbot(VisitorCenter.Greeter):
    """
    Chatbot is an Ice servant that implements Slice interface Greeter.
    """

    def __init__(self, delay: float):
        """
        Initializes a Chatbot servant with a delay.

        Parameters
        ----------
        delay : float
            The delay before returning each greeting.
        """
        self._delay = delay

    # Implements the method greet from the Greeter class generated by the Slice compiler.
    async def greet(self, name: str, current: Ice.Current) -> str:
        print(f"Dispatching greet request {{ name = '{name}' }}")
        await asyncio.sleep(self._delay)
        return f"Hello, {name}!"
