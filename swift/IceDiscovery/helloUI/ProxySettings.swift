// Copyright (c) ZeroC, Inc.

import Foundation

class ProxySettings {
    var connection = ""
    var delay = 0.0
    var timeout = 0.0
    var deliveryMode: DeliveryMode = .twoway
    var methodIndex = 0 {
        didSet {
            deliveryMode = updateDeliveryMode(index: methodIndex)
        }
    }

    func updateDeliveryMode(index: Int) -> DeliveryMode {
        return DeliveryMode(rawValue: index) ?? .twoway
    }

    var isBatched: Bool {
        return DeliveryMode.batchedCases.contains(deliveryMode)
    }

    enum DeliveryMode: Int, CaseIterable {
        static var batchedCases: [DeliveryMode] {
            return [.onewayBatch, .onewaySecureBatch, .datagramBatch]
        }

        case twoway
        case twowaySecure
        case oneway
        case onewayBatch
        case onewaySecure
        case onewaySecureBatch
        case datagram
        case datagramBatch
    }
}
