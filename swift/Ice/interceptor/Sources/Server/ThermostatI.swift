//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice

class ThermostatI: Thermostat {
    private var _temperature: Float = 23.5

    func getTemp(current _: Ice.Current) -> Float {
        return _temperature
    }

    func setTemp(temp: Float, current _: Ice.Current) {
        _temperature = temp
    }

    func shutdown(current: Ice.Current) {
        print("Shutting down...")
        current.adapter!.getCommunicator().shutdown()
    }
}
