//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Darwin
import Ice
import PromiseKit
import UIKit

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

let hostnameKey = "hostnameKey"

class ViewController: UIViewController,
    UIPickerViewDataSource,
    UIPickerViewDelegate,
    UITextFieldDelegate,
    UIAlertViewDelegate {
    @IBOutlet var sayHelloButton: UIButton!
    @IBOutlet var shutdownButton: UIButton!
    @IBOutlet var flushButton: UIButton!
    @IBOutlet var useDiscovery: UISwitch!
    @IBOutlet var hostnameTextField: UITextField!
    @IBOutlet var statusLabel: UILabel!
    @IBOutlet var modePicker: UIPickerView!
    @IBOutlet var timeoutSlider: UISlider!
    @IBOutlet var delaySlider: UISlider!
    @IBOutlet var activity: UIActivityIndicatorView!

    var communicator: Ice.Communicator!
    var helloPrx: HelloPrx!
    var deliveryMode: DeliveryMode!
    var hostname: String!
    var timeout: Int32!
    var discovery: Bool!

    override func viewDidLoad() {
        super.viewDidLoad()

        do {
            var initData = Ice.InitializationData()
            let properties = Ice.createProperties()
            properties.setProperty(key: "Ice.Plugin.IceDiscovery", value: "1")
            properties.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
            properties.setProperty(key: "IceSSL.CheckCertName", value: "0")
            properties.setProperty(key: "IceSSL.DefaultDir", value: "certs")
            properties.setProperty(key: "IceSSL.CAs", value: "cacert.der")
            properties.setProperty(key: "IceSSL.CertFile", value: "client.p12")
            properties.setProperty(key: "IceSSL.Password", value: "password")
            initData.properties = properties

            communicator = try Ice.initialize(initData)
        } catch {
            fatalError()
        }
    }

    @IBAction func sayHello() {
        do {
            try updateProxy()
            if helloPrx == nil {
                return
            }

            let delay = Int32(delaySlider.value * 1000) // Convert to ms.
            if deliveryMode != .OnewayBatch,
                deliveryMode != .OnewaySecureBatch,
                deliveryMode != .DatagramBatch {
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

    @IBAction func flushBatch() {
        if helloPrx == nil {
            return
        }

        flushButton.isEnabled = true
        flushButton.alpha = 0.5
        firstly {
            helloPrx.ice_flushBatchRequestsAsync()
        }.done {
            self.statusLabel.text = "Flushed batch requests"
        }.catch { error in
            self.exception(error)
        }
    }

    @IBAction func setUseDiscovery() {
        if useDiscovery.isOn {
            hostnameTextField.isEnabled = false
            hostnameTextField.text = ""
        } else {
            hostnameTextField.isEnabled = true
            hostnameTextField.text = UserDefaults.standard.string(forKey: hostnameKey)
        }
    }

    @IBAction func shutdown() {
        do {
            try updateProxy()

            if helloPrx == nil {
                return
            }

            if deliveryMode != .OnewayBatch,
                deliveryMode != .OnewaySecureBatch,
                deliveryMode != .DatagramBatch {
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
                }
            } else {
                try helloPrx.shutdown()
                queuedRequest("shutdown")
            }
        } catch {
            exception(error)
        }
    }

    func numberOfComponents(in _: UIPickerView) -> Int {
        return 1
    }

    func pickerView(_: UIPickerView, numberOfRowsInComponent _: Int) -> Int {
        return 8
    }

    func pickerView(_: UIPickerView, titleForRow row: Int, forComponent _: Int) -> String? {
        guard let mode = DeliveryMode(rawValue: row) else {
            return nil
        }

        switch mode {
        case .Twoway:
            return "Twoway"
        case .TwowaySecure:
            return "Twoway Secure"
        case .ModeOneway:
            return "Oneway"
        case .OnewayBatch:
            return "Oneway Batch"
        case .OnewaySecure:
            return "Oneway Secure"
        case .OnewaySecureBatch:
            return "Oneway Secure Batch"
        case .Datagram:
            return "Datagram"
        case .DatagramBatch:
            return "Datagram Batch"
        }
    }

    func updateProxy() throws {
        let t = Int32(timeoutSlider.value * 1000) // Convert to ms.
        guard let mode = DeliveryMode(rawValue: modePicker.selectedRow(inComponent: 0)) else {
            fatalError()
        }

        if helloPrx != nil,
            hostnameTextField.text == hostname,
            timeout == t,
            deliveryMode == mode,
            discovery == useDiscovery.isOn {
            return
        }

        deliveryMode = mode
        timeout = t
        hostname = hostnameTextField.text
        discovery = useDiscovery.isOn

        if discovery == false, hostname == "" {
            helloPrx = nil
            return
        }

        var s: String
        if discovery {
            s = "hello"
        } else {
            s = "hello:tcp -h \"\(hostname!)\" -p 10000:" +
                "ssl -h \"\(hostname!)\" -p 10001:" +
                "udp -h \"\(hostname!)\" -p 10000"
        }

        UserDefaults.standard.set(hostname, forKey: hostnameKey)
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

        if timeout != 0 {
            helloPrx = helloPrx.ice_invocationTimeout(timeout)
        }
    }

    func sendingRequest() {
        sayHelloButton.isEnabled = false
        shutdownButton.isEnabled = false
        statusLabel.text = "Sending request"
        activity.startAnimating()
    }

    func requestSent() {
        if useDiscovery.isOn {
            if let connection = helloPrx.ice_getCachedConnection() {
                do {
                    // Loop through the connection informations until we find an IPConnectionInfo class.
                    for info in sequence(first: try connection.getInfo(), next: { $0.underlying }) {
                        if let ipinfo = info as? IPConnectionInfo {
                            hostnameTextField.text = ipinfo.remoteAddress
                            UserDefaults.standard.set(hostnameTextField.text, forKey: hostnameKey)
                            break
                        }
                    }
                } catch {
                    // Ignore.
                }
            }
        }

        if deliveryMode == .Twoway || deliveryMode == .TwowaySecure {
            statusLabel.text = "Waiting for response"
        } else {
            ready()
        }
    }

    func ready() {
        sayHelloButton.isEnabled = true
        shutdownButton.isEnabled = true
        statusLabel.text = "Ready"
        activity.stopAnimating()
    }

    func queuedRequest(_ name: String) {
        flushButton.isEnabled = true
        flushButton.alpha = 1.0
        statusLabel.text = "Queued \(name) request"
    }

    func flushBatchSend() {
        flushButton.isEnabled = false
        flushButton.alpha = 0.5
        statusLabel.text = "Flushed batch requests"
    }

    func exception(_ ex: Error) {
        let alert = UIAlertController(title: "Error", message: "\(ex)", preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        present(alert, animated: true, completion: nil)
        ready()
    }

    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        return textField.resignFirstResponder()
    }
}
