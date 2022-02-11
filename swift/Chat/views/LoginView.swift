//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import SwiftUI

struct LoginView: View {
    
    @ObservedObject var client = Client()
    @State var showingUsers = false
    @State var showingAlert = false
    @State var loginError: String? = nil

    func login() {}

    init() {
        UserDefaults.standard.register(defaults: [Configuration.Keys.user.rawValue: "",
                                                  Configuration.Keys.password.rawValue: "",
                                                  Configuration.Keys.ssl.rawValue: true,
                                                  Configuration.Keys.rememberMe.rawValue: false])
    }

    var body: some View {
        NavigationView {
            VStack(spacing: 25) {
                Spacer()
                VStack(alignment: .leading, spacing: 25) {
                    Text("ZeroC Chat Demo").font(.title).bold()
                    Text("Username")
                    TextField("Username", text: $client.loginViewModel.username)
                        .textFieldStyle(.roundedBorder)
                    Text("Password")
                    TextField("Password", text: $client.loginViewModel.password)
                        .textFieldStyle(.roundedBorder)
                    Toggle(isOn: $client.loginViewModel.rememberMe) {
                        Text("Remember me")
                    }.tint(Color.activeBlue)
                }
                .padding()
                .offset(y: -35)
                HStack {
                    Spacer()
                    Button("Login") {
                        client.attemptLogin { error in
                            showingAlert = error != nil
                            loginError = error
                        }
                    }
                    .padding()
                    .frame(maxWidth: .infinity, maxHeight: 55)
                    .background(client.loginViewModel.isSigninComplete ? Color.activeBlue : Color.disabledBlue)
                        .accentColor(.white)
                        .cornerRadius(10.0)
                        .disabled(!client.loginViewModel.isSigninComplete)
                        .alert(isPresented: $showingAlert) {
                            Alert(title: Text("Error"),
                                              message: Text(loginError ?? ""),
                                              dismissButton: .default(Text("Got it!")))
                        }
                    Spacer()
                }

                NavigationLink("Chat View", destination: MessagesView().environmentObject(client).navigationTitle("Messages"), isActive: $client.isLoggedIn).hidden()
                    .onReceive(NotificationCenter.default.publisher(for: UIApplication.didEnterBackgroundNotification), perform: { _ in
                        client.destroySession()
                    })
                Spacer()
            }
            .padding()
        }.navigationViewStyle(.stack)
    }
}

struct LoginView_Previews: PreviewProvider {
    static var previews: some View {
        LoginView()
            .previewDevice("iPhone 13 Pro")
    }
}

extension View {
    @ViewBuilder func isHidden(_ isHidden: Bool) -> some View {
        if isHidden {
            self.hidden()
        } else {
            self
        }
    }
}

extension Color {
    static let activeBlue = Color(UIColor(red: 0.27, green: 0.50, blue: 0.82, alpha: 1.00))
    static let disabledBlue = Color(UIColor(red: 0.64, green: 0.75, blue: 0.91, alpha: 1.00))
}
