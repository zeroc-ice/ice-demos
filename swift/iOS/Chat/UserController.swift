//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import UIKit

class UserController: UITableViewController {
    @IBOutlet var usersTableView: UITableView!
    var users: [ChatUser] = []

    func numberOfSectionsInTableView(_: UITableView) -> Int {
        return 1
    }

    override func tableView(_: UITableView, numberOfRowsInSection _: Int) -> Int {
        return users.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let myIdentifier = "MyIdentifier"

        var cell = tableView.dequeueReusableCell(withIdentifier: myIdentifier)

        if cell == nil {
            cell = UITableViewCell(style: .default, reuseIdentifier: myIdentifier)
        }
        cell!.textLabel?.text = users[indexPath.row].displayName
        return cell!
    }

    override func tableView(_: UITableView, willSelectRowAt _: IndexPath) -> IndexPath? {
        return nil
    }
}
