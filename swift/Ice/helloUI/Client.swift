//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Combine
import Ice
import PromiseKit

class Client: ObservableObject {
    private var helloPrx: HelloPrx!
    private var communicator: Ice.Communicator

    // For Button view bindings
    @Published var helloEnabled: Bool = true
    @Published var shutdownEnabled = true
    @Published var flushEnabled = false
    @Published var proxySettings: ProxySettings = .init() {
        didSet {
            do { try updateProxy() } catch {
                exception(error)
            }
        }
    }

    // For Error bindings
    @Published var error: Error?
    @Published var showingError = false

    // For Status bindings
    @Published var isSpinning = false
    @Published var statusMessage = ""

    init() {
        communicator = Client.loadCommunicator()
    }

    func exception(_ err: Error) {
        error = err
        showingError = true
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
        initData.properties = properties
        do {
            return try Ice.initialize(initData)
        } catch {
            print(error)
            fatalError()
        }
    }

    func flushBatchSend() {
        flushEnabled = false
        statusMessage = "Flushed batch requests"
    }

    func queuedRequest(_ name: String) {
        flushEnabled = true
        statusMessage = "Queued \(name) request"
    }

    func ready() {
        helloEnabled = true
        shutdownEnabled = true
        statusMessage = "Ready"
        isSpinning = false
    }

    func requestSent() {
        if proxySettings.deliveryMode == .Twoway || proxySettings.deliveryMode == .TwowaySecure {
            statusMessage = "Waiting for response"
        } else {
            ready()
        }
    }

    func sayHello() {
        do {
            let delay = Int32(proxySettings.delay)
            if proxySettings.deliveryMode != .OnewayBatch,
               proxySettings.deliveryMode != .OnewaySecureBatch,
               proxySettings.deliveryMode != .DatagramBatch
            {
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
            } else {
                // Batch requests get enqueued locally thus this call is non blocking
                try helloPrx.sayHello(delay)
                queuedRequest("hello")
            }
        } catch {
            exception(error)
        }
    }

    func sendingRequest() {
        helloEnabled = false
        shutdownEnabled = false
        statusMessage = "Sending request"
        isSpinning = true
    }

    func shutdown() {
        do {
            if proxySettings.deliveryMode != .OnewayBatch,
               proxySettings.deliveryMode != .OnewaySecureBatch,
               proxySettings.deliveryMode != .DatagramBatch
            {
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
            } else {
                try helloPrx.shutdown()
                queuedRequest("shutdown")
            }
        } catch {
            exception(error)
            ready()
        }
    }

    func updateProxy() throws {
        if proxySettings.hostname == "" {
            return
        }

        let s = "hello:tcp -h \"\(proxySettings.hostname)\" -p 10000:" +
            "ssl -h \"\(proxySettings.hostname)\" -p 10001:" +
            "udp -h \"\(proxySettings.hostname)\" -p 10000"

        helloPrx = uncheckedCast(prx: try communicator.stringToProxy(s)!, type: HelloPrx.self)

        switch proxySettings.deliveryMode {
        case .Twoway:
            helloPrx = helloPrx.ice_twoway()
        case .TwowaySecure:
            helloPrx = helloPrx.ice_twoway().ice_secure(true)
        case .ModeOneway:
            helloPrx = helloPrx.ice_oneway()
        case .OnewayBatch:
            helloPrx = helloPrx.ice_batchOneway()
        case .OnewaySecure:
            helloPrx = helloPrx.ice_oneway().ice_secure(true)
        case .OnewaySecureBatch:
            helloPrx = helloPrx.ice_batchOneway().ice_secure(true)
        case .Datagram:
            helloPrx = helloPrx.ice_datagram()
        case .DatagramBatch:
            helloPrx = helloPrx.ice_batchDatagram()
        }

        if proxySettings.timeout != 0 {
            helloPrx = helloPrx.ice_invocationTimeout(Int32(proxySettings.timeout))
        }
    }
}
