//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import UIKit

class EditController: UIViewController {
    @IBOutlet var textField: UITextField!
    var initialValue: String!

    override func viewDidLoad() {
        super.viewDidLoad()
        textField.placeholder = title
        textField.text = initialValue
    }

    func startEdit(_ name: String, value: String) {
        title = name
        initialValue = value
    }
}
