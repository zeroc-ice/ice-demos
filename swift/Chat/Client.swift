//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Glacier2
import Ice
import PromiseKit

class Client: ObservableObject {
    // MARK: - Public Properties

    var session: ChatSessionPrx?
    var router: Glacier2.RouterPrx?
    var callbackProxy: ChatRoomCallbackPrx?
    var acmTimeout: Int32 = 0

    // MARK: - Private Properties

    private var communicator: Ice.Communicator?

    // Bindings
    @Published var messages: [ChatMessage] = []
    @Published var users: [ChatUser] = []
    @Published var isLoggedIn: Bool = false
    @Published var loginViewModel = LoginViewModel()
    @Published var currentUser: ChatUser! {
        didSet { isLoggedIn = (currentUser != nil) }
    }

    // MARK: - Client management functions

    func setup(communicator: Ice.Communicator,
               session: ChatSessionPrx,
               acmTimeout: Int32,
               router: Glacier2.RouterPrx,
               category: String) throws
    {
        self.communicator = communicator
        self.session = session
        self.router = router
        self.acmTimeout = acmTimeout

        let adapter = try communicator.createObjectAdapterWithRouter(name: "ChatDemo.Client", rtr: router)
        try adapter.activate()
        let prx = try adapter.add(servant: ChatRoomCallbackInterceptor(ChatRoomCallbackDisp(self)),
                                  id: Ice.Identity(name: UUID().uuidString, category: category))
        callbackProxy = uncheckedCast(prx: prx, type: ChatRoomCallbackPrx.self)
    }

    func attemptLogin(completionBlock: @escaping (String?) -> Void) {
        let prop = Ice.createProperties()
        prop.setProperty(key: "Ice.Plugin.IceSSL", value: "1")
        prop.setProperty(key: "Ice.Default.Router",
                         value: "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2 -t 10000")
        prop.setProperty(key: "IceSSL.UsePlatformCAs", value: "1")
        prop.setProperty(key: "IceSSL.CheckCertName", value: "1")
        prop.setProperty(key: "IceSSL.VerifyDepthMax", value: "5")
        prop.setProperty(key: "Ice.ACM.Client.Timeout", value: "0")
        prop.setProperty(key: "Ice.RetryIntervals", value: "-1")

        var initData = Ice.InitializationData()
        initData.properties = prop

        loginViewModel.connecting = true

        var chatsession: ChatSessionPrx?
        var acmTimeout: Int32 = 0
        do {
            communicator = try Ice.initialize(initData)
            let router = uncheckedCast(prx: communicator!.getDefaultRouter()!, type: Glacier2.RouterPrx.self)

            router.createSessionAsync(userId: loginViewModel.username, password: loginViewModel.password)
                .then { session -> Promise<Int32> in
                    precondition(session != nil)
                    chatsession = uncheckedCast(prx: session!, type: ChatSessionPrx.self)
                    return router.getACMTimeoutAsync()
                }.then { timeout -> Promise<String> in
                    acmTimeout = timeout
                    return router.getCategoryForClientAsync()
                }.done { [self] category in

                    try setup(communicator: self.communicator!,
                              session: chatsession!,
                              acmTimeout: acmTimeout,
                              router: router,
                              category: category)
                    loginViewModel.connecting = false
                    currentUser = ChatUser(name: loginViewModel.username.lowercased())
                    completionBlock(nil)
                }.catch { err in
                    if let ex = err as? Glacier2.CannotCreateSessionException {
                        completionBlock("Session creation failed: \(ex.reason)")
                    } else if let ex = err as? Glacier2.PermissionDeniedException {
                        completionBlock("Login failed: \(ex.reason)")
                    } else if let ex = err as? Ice.EndpointParseException {
                        completionBlock("Invalid router: \(ex)")
                    } else {
                        completionBlock("Error: \(err)")
                    }
                }
        } catch {
            completionBlock(error.localizedDescription)
        }
    }

    func destroySession() {
        messages = []
        users = []
        currentUser = nil
        callbackProxy = nil

        if let communicator = communicator {
            self.communicator = nil
            let router = self.router
            self.router = nil
            session = nil
            //
            // Destroy the session and the communicator asyncrhonously
            // to avoid blocking the main thread.
            //
            DispatchQueue.global().async {
                do {
                    try router?.destroySession()
                } catch {
                    print("Destory issue \(error.localizedDescription)")
                }
                communicator.destroy()
            }
        }
    }
}

// MARK: - ChatRoomCallBack functions

extension Client: ChatRoomCallback {
    func `init`(users: StringSeq, current _: Current) throws {
        self.users = users.map { ChatUser(name: $0) }
    }

    func send(timestamp: Int64, name: String, message: String, current _: Current) throws {
        append(ChatMessage(text: message, who: name, timestamp: timestamp))
    }

    func join(timestamp: Int64, name: String, current _: Current) throws {
        append(ChatMessage(text: "\(name) joined.", who: "System Message", timestamp: timestamp))
        users.append(ChatUser(name: name))
    }

    func leave(timestamp: Int64, name: String, current _: Current) throws {
        append(ChatMessage(text: "\(name) left.", who: "System Message", timestamp: timestamp))
        users.removeAll(where: { $0.displayName == name })
    }

    func append(_ message: ChatMessage) {
        if messages.count > 100 { // max 100 messages
            messages.remove(at: 0)
        }
        messages.append(message)
    }
}

class ChatRoomCallbackInterceptor: Disp {
    private let servantDisp: Disp
    init(_ servantDisp: Disp) {
        self.servantDisp = servantDisp
    }

    func dispatch(request: Request, current: Current) throws -> Promise<Ice.OutputStream>? {
        // Dispatch the request to the main thread in order to modify the UI in a thread safe manner.
        return try DispatchQueue.main.sync {
            try self.servantDisp.dispatch(request: request, current: current)
        }
    }
}
