// Copyright (c) ZeroC, Inc.

import SwiftUI

struct ProxySettingsView: View {
    @EnvironmentObject var client: Client

    @Binding var connection: String
    @Binding var delay: Double
    @Binding var timeout: Double
    @Binding var methodIndex: Int

    let deliveryMethodOptions = [
        "Twoway", "Twoway Secure", "Oneway", "Oneway Batch",
        "Oneway Secure", "Oneway Secure Batch", "Datagram", "Datagram Batch",
    ]
    var body: some View {
        Section {
            TextField("localhost", text: $connection)
                .autocapitalization(.none)
                .disableAutocorrection(true)
            Picker(selection: $methodIndex, label: Text("Delivery Method")) {
                ForEach(0..<deliveryMethodOptions.count) {
                    Text(self.deliveryMethodOptions[$0])
                }
            }
            HStack {
                Text("Delay")
                    .frame(width: 80, alignment: .leading)
                Spacer()
                Slider(
                    value: $delay,
                    in: 0...10000,
                    step: 1000
                ) {
                    Text("Delay")
                } minimumValueLabel: {
                    Text("0 s")
                } maximumValueLabel: {
                    Text("10 s")
                }
            }
            HStack {
                Text("Timeout")
                    .frame(width: 80, alignment: .leading)
                Spacer()
                Slider(
                    value: $timeout,
                    in: 0...10000,
                    step: 1000
                ) {
                    Text("Timeout")
                } minimumValueLabel: {
                    Text("0 s")
                } maximumValueLabel: {
                    Text("10 s")
                }
            }
        } header: {
            Text("Proxy Settings")
        } footer: {
            Text("*The proxy settings are explained in the README.md")
        }
        #if os(macOS)
            .padding()
        #endif
    }
}
