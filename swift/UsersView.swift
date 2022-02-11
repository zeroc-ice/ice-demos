//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import SwiftUI

struct UsersView: View {
    var users: [ChatUser]

    var body: some View {
        List(users) { user in
            Text(user.displayName)
        }
    }
}
