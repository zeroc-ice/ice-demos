//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import SwiftUI

struct UsersView: View {
    @Environment(\.presentationMode) private var presentationMode
    var users: [ChatUser]

    var body: some View {
        VStack(alignment: .leading) {
            Button("Dismiss") {
                self.presentationMode.wrappedValue.dismiss()
            }
            .offset(y: 5)
            .buttonStyle(.bordered)
            .padding()
            List(users) { user in
                Text(user.displayName)
            }
        }
    }
}
