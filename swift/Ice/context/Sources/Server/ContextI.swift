//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice

class ContextI: Context {
    func call(current: Ice.Current) {
        let type = current.ctx["type"] ?? "None"
        print("Type = \(type)")
    }

    func shutdown(current: Ice.Current) {
        print("Shutting down...")
        current.adapter!.getCommunicator().shutdown()
    }
}
