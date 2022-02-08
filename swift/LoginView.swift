//
//  LoginView.swift
//  IOSChat
//
//  Created by Reece Humphreys on 2/8/22.
//

import SwiftUI

struct LoginView: View {
    
    @ObservedObject var loginViewModel = LoginViewModel()
    
    func login(){
        
    }
    
    init(){
        UserDefaults.standard.register(defaults: [Configuration.Keys.user.rawValue: "",
                                                  Configuration.Keys.password.rawValue: "",
                                                  Configuration.Keys.ssl.rawValue: true,
                                                  Configuration.Keys.rememberMe.rawValue: false])
    }

    
//    func exception(_ message: String) {
//        connecting = false
//        if (usernameField.text ?? "").isEmpty {
//            loginButton.isEnabled = false
//            loginButton.alpha = 0.5
//        } else {
//            loginButton.isEnabled = true
//            loginButton.alpha = 1.0
//        }
//
//        let alert = UIAlertController(title: "Error", message: message, preferredStyle: .alert)
//        alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
//        present(alert, animated: true, completion: nil)
//    }
    
    var body: some View {
        VStack(spacing: 35) {
            Spacer()
            VStack(alignment: .leading, spacing: 35) {
                Text("ZeroC Chat App").font(.title).bold()
                Text("Username")
                TextField("Username", text: $loginViewModel.username)
                    .textFieldStyle(.roundedBorder)
                Text("Password")
                TextField("Password", text: $loginViewModel.password)
                    .textFieldStyle(.roundedBorder)
                Toggle(isOn: $loginViewModel.rememberMe) {
                    Text("Remember me")
                }.accentColor(Color.blue)
            }.padding()
            Spacer()
            Button("Login") {
                loginViewModel.attemptLogin { success in
                    if success {
                        print("Yay")
                    } else {
                        print("Nay")
                    }
                }
            }.frame(maxWidth: .infinity, maxHeight: 50.0)
                .background(loginViewModel.isSigninComplete ? Color.blue : Color.gray)
             .accentColor(.white)
             .cornerRadius(5.0)
             .disabled(!loginViewModel.isSigninComplete)
            Spacer()
        }
        .padding()
    }
}

struct LoginView_Previews: PreviewProvider {
    
    @State static var username = ""
    @State static var password = ""
    @State static var remember = false
    
    static var previews: some View {
        LoginView()
    }
}
