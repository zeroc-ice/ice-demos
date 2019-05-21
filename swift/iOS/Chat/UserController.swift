//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import UIKit
import Ice

class UserController: UITableViewController {
    @IBOutlet weak var usersTableView: UITableView!
    var users: [String] = []

    func numberOfSectionsInTableView(_: UITableView) -> Int {
        return 1
    }

    override func tableView(_: UITableView, numberOfRowsInSection: Int) -> Int {
        return users.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let myIdentifier = "MyIdentifier"

        var cell = tableView.dequeueReusableCell(withIdentifier: myIdentifier)

        if cell == nil {
            cell = UITableViewCell(style: .default, reuseIdentifier: myIdentifier)
        }
        cell!.textLabel?.text = users[indexPath.row]
        return cell!
    }

    override func tableView(_ tableView: UITableView, willSelectRowAt indexPath: IndexPath) -> IndexPath? {
        return nil
    }
}
