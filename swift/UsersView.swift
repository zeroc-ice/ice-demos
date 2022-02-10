//
//  UsersView.swift
//  IOSChat
//
//  Created by Reece Humphreys on 2/10/22.
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
