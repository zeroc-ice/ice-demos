// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <Chat.h>

using namespace std;
using namespace Demo;

namespace
{

// mutex to prevent intertwined cout output
mutex coutMutex;

}

class ChatCallbackI : public ChatCallback
{
public:

    virtual void
    message(string data, const Ice::Current&) override
    {
        lock_guard<mutex> lock(coutMutex);
        cout << data << endl;
    }
};

class ChatClient : public Glacier2::Application
{
public:

    ChatClient() :
        //
        // Since this is an interactive demo we don't want any signal
        // handling.
        //
        Glacier2::Application(Ice::SignalPolicy::NoSignalHandling)
    {
    }

    virtual shared_ptr<Glacier2::SessionPrx>
    createSession() override
    {
        shared_ptr<ChatSessionPrx> sess;
        while(!sess)
        {
            cout << "This demo accepts any user-id / password combination.\n";

            string id;
            cout << "user id: " << flush;
            getline(cin, id);
            id = trim(id);

            string pw;
            cout << "password: " << flush;
            getline(cin, pw);
            pw = trim(pw);

            try
            {
                sess = Ice::uncheckedCast<ChatSessionPrx>(router()->createSession(id, pw));
                break;
            }
            catch(const Glacier2::PermissionDeniedException& ex)
            {
                cout << "permission denied:\n" << ex.reason << endl;
            }
            catch(const Glacier2::CannotCreateSessionException ex)
            {
                cout << "cannot create session:\n" << ex.reason << endl;
            }
        }
        return sess;
    }

    virtual int
    runWithSession(int argc, char*[]) override
    {
        if(argc > 1)
        {
            cerr << appName() << ": too many arguments" << endl;
            return EXIT_FAILURE;
        }

        auto callbackReceiverIdent = createCallbackIdentity("callbackReceiver");
        auto cb = make_shared<ChatCallbackI>();
        auto callback = Ice::uncheckedCast<ChatCallbackPrx>(objectAdapter()->add(cb, callbackReceiverIdent));

        auto sessionPrx = Ice::uncheckedCast<ChatSessionPrx>(session());
        sessionPrx->setCallback(callback);
        menu();

        do
        {
            string s;
            {
                lock_guard<mutex> lock(coutMutex);
                cout << "==> ";
            }
            getline(cin, s);
            s = trim(s);
            if(!s.empty())
            {
                if(s[0] == '/')
                {
                    if(s == "/quit")
                    {
                        break;
                    }
                    menu();
                }
                else
                {
                    sessionPrx->say(s);
                }
            }
        }
        while(cin.good());
        return EXIT_SUCCESS;
    }

private:

    void
    menu()
    {
        lock_guard<mutex> lock(coutMutex);
        cout << "enter /quit to exit." << endl;
    }

    string
    trim(const string& s)
    {
        static const string delims = "\t\r\n ";
        auto last = s.find_last_not_of(delims);
        if(last != string::npos)
        {
            return s.substr(s.find_first_not_of(delims), last+1);
        }
        return s;
    }
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    ChatClient app;
    return app.main(argc, argv, "config.client");
}
