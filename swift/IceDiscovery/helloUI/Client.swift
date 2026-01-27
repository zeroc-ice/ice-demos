//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Combine
import Foundation
import Ice
import PromiseKit

class Client: ObservableObject {
    // MARK: - Public Properties

    // For Error bindings
    @Published var error: Error?
    @Published var showingError = false

    // For Status bindings
    @Published var isSpinning = false
    @Published var statusMessage = ""

    // For Button view bindings
    @Published var helloEnabled: Bool = true
    @Published var shutdownEnabled = true
    @Published var flushEnabled = false

    @Published var proxySettings: ProxySettings = .init() {
        didSet {
            do { try updateProxy() } catch { exception(error) }
        }
    }

    // MARK: - Private Properties

    private var helloPrx: HelloPrx!
    private var communicator: Ice.Communicator

    // MARK: - Initialize the client with a communicator

    init() {
        communicator = Client.loadCommunicator()
        do { try updateProxy() } catch { exception(error) }
    }

    // MARK: - Public functions to send the say hello message, flush the batch requests, and shutdown the hello server

    func sayHello() {
        do {
            let delay = Int32(proxySettings.delay)
            if proxySettings.isBatched {
                // Batch requests get enqueued locally thus this call is non blocking
                try helloPrx.sayHello(delay)
                queuedRequest("hello")
            } else {
                // Non Batched requests get sent asynchronously
                var response = false
                firstly {
                    helloPrx.sayHelloAsync(delay, sentOn: DispatchQueue.main) { _ in
                        if !response {
                            self.requestSent()
                        }
                    }
                }.done {
                    response = true
                    self.ready()
                }.catch { error in
                    response = true
                    self.exception(error)
                }
            }
        } catch {
            exception(error)
        }
    }

    func flushBatch() {
        firstly {
            helloPrx.ice_flushBatchRequestsAsync()
        }.done {
            self.flushBatchSend()
        }.catch { error in
            self.exception(error)
        }
    }

    func shutdown() {
        do {
            if proxySettings.isBatched {
                try helloPrx.shutdown()
                queuedRequest("shutdown")
            } else {
                sendingRequest()
                var response = false
                firstly {
                    helloPrx.shutdownAsync { _ in
                        if !response {
                            self.requestSent()
                        }
                    }
                }.done {
                    response = true
                    self.ready()
                }.catch { error in
                    response = true
                    self.exception(error)
                    self.ready()
                }
            }
        } catch {
            exception(error)
            ready()
        }
    }

    // MARK: - Control the state of the client

    private func exception(_ err: Error) {
        error = err
        showingError = true
    }

    private func flushBatchSend() {
        flushEnabled = false
        statusMessage = "Flushed batch requests"
    }

    private class func loadCommunicator() -> Ice.Communicator {
        var initData = Ice.InitializationData()
        let properties = Ice.createProperties()

        properties.setProperty(key: "Ice.Plugin.IceDiscovery", value: "0")
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

    private func queuedRequest(_ name: String) {
        flushEnabled = true
        statusMessage = "Queued \(name) request"
    }

    private func ready() {
        helloEnabled = true
        shutdownEnabled = true
        statusMessage = "Ready"
        isSpinning = false
    }

    private func requestSent() {
        if let connection = helloPrx.ice_getCachedConnection() {
            do {
                // Loop through the connection informations until we find an IPConnectionInfo class.
                for info in sequence(first: try connection.getInfo(), next: { $0.underlying }) {
                    if let ipinfo = info as? IPConnectionInfo {
                        proxySettings.connection = ipinfo.remoteAddress
                        break
                    }
                }
            } catch {
                // Ignore.
            }
        }
        if proxySettings.deliveryMode == .twoway || proxySettings.deliveryMode == .twowaySecure {
            statusMessage = "Waiting for response"
        } else {
            ready()
        }
    }

    private func sendingRequest() {
        helloEnabled = false
        shutdownEnabled = false
        statusMessage = "Sending request"
        isSpinning = true
    }

    private func updateProxy() throws {
        helloPrx = uncheckedCast(prx: try communicator.propertyToProxy("HelloProxy")!, type: HelloPrx.self)

        switch proxySettings.deliveryMode {
        case .twoway:
            helloPrx = helloPrx.ice_twoway()
        case .twowaySecure:
            helloPrx = helloPrx.ice_twoway().ice_secure(true)
        case .oneway:
            helloPrx = helloPrx.ice_oneway()
        case .onewayBatch:
            helloPrx = helloPrx.ice_batchOneway()
        case .onewaySecure:
            helloPrx = helloPrx.ice_oneway().ice_secure(true)
        case .onewaySecureBatch:
            helloPrx = helloPrx.ice_batchOneway().ice_secure(true)
        case .datagram:
            helloPrx = helloPrx.ice_datagram()
        case .datagramBatch:
            helloPrx = helloPrx.ice_batchDatagram()
        }

        if proxySettings.timeout != 0 {
            helloPrx = helloPrx.ice_invocationTimeout(Int32(proxySettings.timeout))
        }
    }
}
