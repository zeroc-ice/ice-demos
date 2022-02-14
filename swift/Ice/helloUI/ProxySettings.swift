//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation

class ProxySettings {
    var hostname = ""
    var delay = 0.0
    var timeout = 0.0
    var deliveryMode: DeliveryMode = .Twoway
    var methodIndex = 0 {
        didSet {
            deliveryMode = updateDeliveryMode(index: methodIndex)
        }
    }

    func updateDeliveryMode(index: Int) -> DeliveryMode {
        return DeliveryMode(rawValue: index) ?? .Twoway
    }

    enum DeliveryMode: Int {
        case Twoway = 0
        case TwowaySecure = 1
        case ModeOneway = 2
        case OnewayBatch = 3
        case OnewaySecure = 4
        case OnewaySecureBatch = 5
        case Datagram = 6
        case DatagramBatch = 7
    }
}
