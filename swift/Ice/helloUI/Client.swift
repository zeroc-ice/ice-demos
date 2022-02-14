//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Combine
import Ice
import PromiseKit

class Client: ObservableObject {
    var helloPrx: HelloPrx!
    var communicator: Ice.Communicator!

    let hostnameKey = "hostnameKey"

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

    // Error bindings
    @Published var error: Error?
    @Published var showingError = false

    // Status bindings
    @Published var isSpinning = false
    @Published var statusMessage = ""

    func configure(comm: Ice.Communicator) {
        communicator = comm
    }

    func exception(_ err: Error) {
        error = err
        showingError = true
    }

    func sayHello() {
        do {
            if helloPrx == nil {
                return
            }

            let delay = Int32(proxySettings.delay)
            if proxySettings.deliveryMode != .OnewayBatch,
               proxySettings.deliveryMode != .OnewaySecureBatch,
               proxySettings.deliveryMode != .DatagramBatch
            {
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
                try helloPrx.sayHello(delay)
                queuedRequest("hello")
            }
        } catch {
            exception(error)
        }
    }

    func flushBatch() {
        if helloPrx == nil {
            return
        }
        flushEnabled = true
        firstly {
            helloPrx.ice_flushBatchRequestsAsync()
        }.done {
            self.statusMessage = "Flushed batch requests"
        }.catch { error in
            self.exception(error)
        }
    }

    func shutdown() {
        do {
            if helloPrx == nil {
                return
            }

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
        let mode = proxySettings.deliveryMode

        if proxySettings.hostname == "" {
            helloPrx = nil
            return
        }

        let s = "hello:tcp -h \"\(proxySettings.hostname)\" -p 10000:" +
            "ssl -h \"\(proxySettings.hostname)\" -p 10001:" +
            "udp -h \"\(proxySettings.hostname)\" -p 10000"

        UserDefaults.standard.set(proxySettings.hostname, forKey: hostnameKey)
        helloPrx = uncheckedCast(prx: try communicator.stringToProxy(s)!, type: HelloPrx.self)

        switch mode {
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

    func sendingRequest() {
        helloEnabled = false
        shutdownEnabled = false
        statusMessage = "Sending request"
        isSpinning = true
    }

    func requestSent() {
        if let connection = helloPrx.ice_getCachedConnection() {
            do {
                // Loop through the connection informations until we find an IPConnectionInfo class.
                for info in sequence(first: try connection.getInfo(), next: { $0.underlying }) {
                    if let ipinfo = info as? IPConnectionInfo {
                        proxySettings.hostname = ipinfo.remoteAddress
                        UserDefaults.standard.set(proxySettings.hostname, forKey: hostnameKey)
                        break
                    }
                }
            } catch {
                // Ignore.
            }
        }

        if proxySettings.deliveryMode == .Twoway || proxySettings.deliveryMode == .TwowaySecure {
            statusMessage = "Waiting for response"
        } else {
            ready()
        }
    }

    func ready() {
        helloEnabled = true
        shutdownEnabled = true
        statusMessage = "Ready"
        isSpinning = false
    }

    func queuedRequest(_ name: String) {
        flushEnabled = true
        statusMessage = "Queued \(name) request"
    }

    func flushBatchSend() {
        flushEnabled = false
        statusMessage = "Flushed batch requests"
    }
}
