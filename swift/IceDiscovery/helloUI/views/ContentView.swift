//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Darwin
import Ice
import PromiseKit
import SwiftUI

struct ContentView: View {
    func loadCommunicator() -> Ice.Communicator {
        var initData = Ice.InitializationData()
        let properties = Ice.createProperties()
        properties.setProperty(key: "Ice.Plugin.IceDiscovery", value: "1")
        properties.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
        properties.setProperty(key: "IceSSL.CheckCertName", value: "0")
        properties.setProperty(key: "IceSSL.DefaultDir", value: "certs")
        properties.setProperty(key: "IceSSL.CAs", value: "cacert.der")
        properties.setProperty(key: "IceSSL.CertFile", value: "client.p12")
        properties.setProperty(key: "IceSSL.Password", value: "password")
        properties.setProperty(key: "HelloProxy", value: "hello")
        initData.properties = properties
        do {
            return try Ice.initialize(initData)
        } catch {
            print(error)
            fatalError()
        }
    }

    var body: some View {
        #if os(iOS)
            NavigationView {
                FormView(communicator: loadCommunicator())
            }
            .navigationViewStyle(.stack)
        #else
            FormView(communicator: loadCommunicator())
        #endif
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            ContentView()
                .previewDevice("iPhone 13 mini")
                .previewDisplayName("iPhone 13 mini")
                .previewInterfaceOrientation(.portrait)
            ContentView()
                .previewDevice("iPhone 13 Pro")
                .previewDisplayName("iPhone 13 Pro")
                .previewInterfaceOrientation(.portrait)
            ContentView()
                .previewDevice("iPhone 13 Pro Max")
                .previewDisplayName("iPhone 13 Pro Max")
                .previewInterfaceOrientation(.portrait)
            ContentView()
                .previewDevice("iPad Air (4th generation)")
                .previewDisplayName("iPad Air")
                .previewInterfaceOrientation(.landscapeLeft)
        }
    }
}
