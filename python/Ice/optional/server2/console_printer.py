# Copyright (c) ZeroC, Inc.

import Ice
from ClearSky import AtmosphericConditions, WeatherStation


class ConsolePrinter(WeatherStation):
    """
    ConsolePrinter is an Ice servant that implements Slice interface WeatherStation.
    """

    # Implements the method report from the WeatherStation class generated by the Slice compiler.
    # This variant is the synchronous implementation.
    def report(self, sensorId: str, reading: AtmosphericConditions, current: Ice.Current) -> None:
        print(f"{sensorId} reported {reading} to station v2")
