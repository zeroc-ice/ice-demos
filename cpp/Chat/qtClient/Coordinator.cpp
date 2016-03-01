// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Chat.h>
#include <ChatSession.h>

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QSettings>

#include <Coordinator.h>
#include <MainView.h>
#include <ChatView.h>
#include <LoginView.h>
#include <Util.h>

using namespace std;
using namespace Chat;

class CustomEvent : public QEvent
{
public:

    CustomEvent(const Ice::DispatcherCallPtr& call, const Ice::ConnectionPtr& connection) :
        QEvent(QEvent::Type(CUSTOM_EVENT_TYPE)),
        _call(call),
        _connection(connection)
    {
    }

    Ice::DispatcherCallPtr
    call() const
    {
        return _call;
    }

    Ice::ConnectionPtr
    connection() const
    {
        return _connection;
    }

private:

    const Ice::DispatcherCallPtr _call;
    const Ice::ConnectionPtr _connection;
};

std::string
LoginInfo::username() const
{
    IceUtil::Mutex::Lock sync(_mutex);
    return _username;
}

void
LoginInfo::setUsername(const std::string& username)
{
    IceUtil::Mutex::Lock sync(_mutex);
    _username = username;
}

std::string
LoginInfo::password() const
{
    IceUtil::Mutex::Lock sync(_mutex);
    return _password;
}

void
LoginInfo::setPassword(const std::string& password)
{
    IceUtil::Mutex::Lock sync(_mutex);
    _password = password;
}

void
LoginInfo::load()
{
    IceUtil::Mutex::Lock sync(_mutex);
#ifdef _WIN32
    QString envUserName;
    vector<wchar_t> v(256);
    DWORD ret = GetEnvironmentVariableW(L"USERNAME", &v[0], static_cast<DWORD>(v.size()));
    if(ret >= v.size())
    {
        v.resize(ret + 1);
        ret = GetEnvironmentVariableW(L"USERNAME", &v[0], static_cast<DWORD>(v.size()));
    }

    if(ret > 0)
    {
        envUserName = QString::fromUtf16(reinterpret_cast<ushort*>(&v[0]), ret);
    }
    else
    {
        envUserName = "";
    }
#else
    QString envUserName(getenv("USER"));
#endif
    QSettings settings("ZeroC", "Chat Demo");
    settings.beginGroup("LoginInfo");
    _username = settings.value("username", envUserName).toString().toUtf8().data();
    settings.endGroup();
}

void
LoginInfo::save()
{
    IceUtil::Mutex::Lock sync(_mutex);
    QSettings settings("ZeroC", "Chat Demo");
    settings.beginGroup("LoginInfo");
    settings.setValue("username", _username.c_str());
    settings.endGroup();
    settings.sync();
}

bool
Coordinator::event(QEvent* event)
{
    if(event->type() != CUSTOM_EVENT_TYPE)
    {
        return QObject::event(event);
    }

    CustomEvent* customEvent = dynamic_cast<CustomEvent*>(event);
    assert(customEvent);
    Ice::DispatcherCallPtr call = customEvent->call();
    assert(call);
    try
    {
        call->run();
    }
    catch(const Ice::Exception& ex)
    {
        ostringstream error;
        error << "Ice::DispatcherCall (Ice::Exception):\n" << ex;
        setError(error.str());
    }
    catch(const exception& ex)
    {
        ostringstream error;
        error << "Ice::DispatcherCall (std::exception):\n" << ex.what();
        setError(error.str());
    }
    catch(const string& ex)
    {
        ostringstream error;
        error << "Ice::DispatcherCall (std::string):\n" << ex;
        setError(error.str());
    }
    catch(const char* ex)
    {
        ostringstream error;
        error << "Ice::DispatcherCall (const char*):\n" << ex;
        setError(error.str());
    }
    catch(...)
    {
        ostringstream error;
        error << "Ice::DispatcherCall (unknown C++ exception).";
        setError(error.str());
    }
    return true;
}

namespace
{

class ChatRoomCallbackI : public Chat::ChatRoomCallback
{

public:

    ChatRoomCallbackI(const CoordinatorPtr& coordinator, const std::string& username) :
        _coordinator(coordinator),
        _username(username)
    {
    }

    virtual void
    init(const Ice::StringSeq& users, const Ice::Current&)
    {
        _coordinator->initEvent(users);
    }

    virtual void
    send(Ice::Long timestamp, const std::string& username, const std::string& message, const Ice::Current&)
    {
        if(username != _username)
        {
            _coordinator->userSayEvent(timestamp, username, message);
        }
    }

    virtual void
    join(Ice::Long timestamp, const std::string& username, const Ice::Current&)
    {
        _coordinator->userJoinEvent(timestamp, username);
    }

    virtual void
    leave(Ice::Long timestamp, const std::string& username, const Ice::Current&)
    {
        _coordinator->userLeaveEvent(timestamp, username);
    }

private:

    const CoordinatorPtr _coordinator;
    const std::string _username;
};

class Dispatcher : public Ice::Dispatcher
{
public:

    Dispatcher(QObject* receiver) :
        _receiver(receiver),
        _exit(false)
    {
    }

    virtual ~Dispatcher()
    {
    }

    void exit()
    {
        _exit = true;
    }

    virtual void dispatch(const Ice::DispatcherCallPtr& call,
                          const Ice::ConnectionPtr& conn)
    {
        assert(_receiver);
        if(_exit) // The GUI is being destroyed, don't use the GUI thread any more
        {
            call->run();
        }
        else
        {
            QCoreApplication::postEvent(_receiver, new CustomEvent(call, conn));
        }
    }

private:

    QObject* _receiver;
    bool _exit;
};
typedef IceUtil::Handle<Dispatcher> DispatcherPtr;

class CoordinatorI;
typedef IceUtil::Handle<CoordinatorI> CoordinatorIPtr;

class CoordinatorI : public Coordinator
{

public:

    class SendCallback : public IceUtil::Shared
    {

    public:

        SendCallback(const CoordinatorIPtr& coordinator, const std::string& username,
                     const std::string& message) :
            _coordinator(coordinator),
            _username(username),
            _message(message)

        {
        }

        void
        response(Ice::Long timestamp)
        {
            _coordinator->userSayEvent(timestamp, _username, _message);
        }

        void
        exception(const Ice::Exception& ex)
        {
            try
            {
                ex.ice_throw();
            }
            catch(const Chat::InvalidMessageException& e)
            {
                ostringstream os;
                os << "<system-message> - " << e.reason;
                _coordinator->appendMessage(os.str());
            }
            catch(const Ice::Exception&)
            {
                _coordinator->destroySession(ex);
            }
        }

    private:

        const CoordinatorIPtr _coordinator;
        const string _username;
        const string _message;
    };

    class ConnectCallback : public IceUtil::Shared
    {

    public:

        ConnectCallback(const CoordinatorIPtr& coordinator, const std::string& username) :
            _coordinator(coordinator),
            _username(username)

        {
        }

        void
        response()
        {
            _coordinator->setState(Connected);
        }

        void
        exception(const Ice::Exception& ex)
        {
            _coordinator->destroySession(ex);
        }

    private:

        const CoordinatorIPtr _coordinator;
        const string _username;
    };

    CoordinatorI(MainView* mainView, LoginView* loginView, ChatView* chatView,
                 UserListModel* userListModel, int argc, char** argv) :
        _mainView(mainView),
        _loginView(loginView),
        _chatView(chatView),
        _userListModel(userListModel),
        _info(new LoginInfo()),
        _maxMessageSize(1024),
        _exit(false)
    {
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties(argc, argv);

        //
        // In OS X we load the IceSSL plug in from the application bundle directory.
        //
#ifdef __APPLE__
        initData.properties->setProperty("Ice.Plugin.IceSSL",
            QString(QApplication::applicationDirPath() + "/../Frameworks/IceSSL:createIceSSL").toUtf8().data());
#endif

        _dispatcher = new Dispatcher(this);
        initData.dispatcher = _dispatcher;

        //
        // Set Ice.Default.Router if not set.
        //
        if(initData.properties->getProperty("Ice.Default.Router").empty())
        {
            initData.properties->setProperty("IceSSL.UsePlatformCAs", "1");
            initData.properties->setProperty("IceSSL.CheckCertName", "1");
            initData.properties->setProperty("Ice.Default.Router",
                                             "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2");
        }

        _factory = new Glacier2::SessionFactoryHelper(initData, this);
        setState(Disconnected);
    }

    void
    createdCommunicator(const Glacier2::SessionHelperPtr& session)
    {
        // Avoid the spurious callback
        if(session != _session)
        {
            return;
        }
    }

    void
    login(const LoginInfoPtr& info)
    {
        setState(Connecting);
        _info = info;
        _session = _factory->connect(info->username(), info->password());
    }

    void
    login()
    {
        setState(Disconnected);
    }

    void
    logout()
    {
        destroySession();
    }

    void
    exit()
    {
        _exit = true;
        _dispatcher->exit();
        Ice::CommunicatorPtr communicator = _session ? _session->communicator() : static_cast<Ice::CommunicatorPtr>(0);

        logout();

        if(communicator)
        {
            communicator->waitForShutdown();
        }

        _factory->destroy();
    }

    void
    connected(const Glacier2::SessionHelperPtr& session)
    {
        //
        // Ignore callbacks after shutdown.
        //
        if(_exit)
        {
            return;
        }

        //
        // Avoid the spurious callback.
        //
        if(_session != session)
        {
            return;
        }

        _info->save();
        _username = formatUsername(_info->username());
        ChatRoomCallbackPrx callback = ChatRoomCallbackPrx::uncheckedCast(
                                                _session->addWithUUID(new ChatRoomCallbackI(this, _username)));
        ChatSessionPrx chat = ChatSessionPrx::uncheckedCast(_session->session());

        try
        {
            Callback_ChatSession_setCallbackPtr cb = newCallback_ChatSession_setCallback(
                    new ConnectCallback(this, _info->username()),
                    &ConnectCallback::response, &ConnectCallback::exception);
            chat->begin_setCallback(callback, cb);
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored the application is being shutdown.
        }
    }

    void
    disconnected(const Glacier2::SessionHelperPtr& session)
    {
        if(_exit)
        {
            return;
        }

        //
        // Avoid the spurious callback
        //
        if(session != _session)
        {
            return;
        }

        _username = "";
        _session = 0;
        setState(Disconnected);
    }

    void
    disconnected(const Glacier2::SessionHelperPtr& session, const Ice::Exception& ex)
    {
        if(_exit)
        {
            return;
        }

        //
        // Avoid the spurious callback
        //
        if(session != _session)
        {
            return;
        }

        _username = "";
        _session = 0;

        ostringstream os;
        if(_state == Connected) // Connection lost while user was chatting
        {

            os << "<system-message> - The connection with the server was unexpectedly lost.\n"
               << "You can try to login again from the File menu.\n";

        }
        else // Connection lost while user was connecting
        {
            os << "<system-message> - The connection with the server was unexpectedly lost.\n"
               << "Try again.\n";
        }
        os << ex;

        setError(os.str());
    }

    void
    connectFailed(const Glacier2::SessionHelperPtr& session, const Ice::Exception& ex)
    {
        //
        // Ignore callbacks during shutdown.
        //
        if(_exit)
        {
            return;
        }

        //
        // Avoid the spurious callback
        //
        if(session != _session)
        {
            return;
        }

        _username = "";
        _session = 0;

        try
        {
            ex.ice_throw();
        }
        catch(const Glacier2::CannotCreateSessionException& ex)
        {
            ostringstream os;
            os << "Login failed (Glacier2::CannotCreateSessionException):\n" << ex.reason;
            setError(os.str());
        }
        catch(const Glacier2::PermissionDeniedException& ex)
        {
            ostringstream os;
            os << "Login failed (Glacier2::PermissionDeniedException):\n" << ex.reason;

            setError(os.str());
        }
        catch(const Ice::Exception& ex)
        {
            ostringstream os;
            os << "Login failed ("<< ex.ice_name() << ").\n"
               << "Please check your configuration.";

            setError(os.str());
        }
        setState(Disconnected);
    }

    virtual void
    setError(const std::string& message)
    {
        if(_state == Connected)
        {
            _chatView->appendMessage(message);
            _mainView->enableLoginAction();
        }
        else
        {
            QMessageBox msgBox(_loginView);
            msgBox.setWindowModality(Qt::WindowModal);
            msgBox.setWindowTitle("Error - Chat Demo");
            msgBox.setText(message.c_str());
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }

    virtual void
    initEvent(const Ice::StringSeq& users)
    {
        for(Ice::StringSeq::const_iterator i = users.begin(); i != users.end(); ++i)
        {
            _userListModel->addElement(*i);
        }
    }

    virtual void
    userJoinEvent(Ice::Long timestamp, const std::string& username)
    {
        _userListModel->addElement(username);

        ostringstream os;
        os << formatTimestamp(timestamp) << " - <system-message> - " << username << " joined.";
        _chatView->appendMessage(os.str());
    }


    virtual void
    userLeaveEvent(Ice::Long timestamp, const std::string& username)
    {
        _userListModel->removeElement(username);

        ostringstream os;
        os << formatTimestamp(timestamp) << " - <system-message> - " << username << " leave.";
        _chatView->appendMessage(os.str());
    }

    virtual void
    userSayEvent(Ice::Long timestamp, const std::string& username, const std::string& message)
    {
        ostringstream os;
        os << formatTimestamp(timestamp) << " - <" + username + "> " << unstripHtml(message);
        _chatView->appendMessage(os.str());
    }

    virtual void
    sendMessage(const std::string& message)
    {
        if(_state != Connected)
        {
            return;
        }

        if(message.size() > _maxMessageSize)
        {
            ostringstream os;
            os << "<system-message> - Message length exceeded, maximum length is "
               << _maxMessageSize << " characters.";
            _chatView->appendMessage(os.str());
            return;
        }

        try
        {
            ChatSessionPrx chat = ChatSessionPrx::uncheckedCast(_session->session());
            chat->begin_send(message,
                             newCallback_ChatSession_send(new SendCallback(this, _username, message),
                                &SendCallback::response, &SendCallback::exception));
        }
        catch(Ice::CommunicatorDestroyedException&)
        {
            //Ignore client session was destroyed.
        }
    }

    virtual void
    setState(ClientState state)
    {
        _state = state;
        if(state == Disconnected)
        {
            if(QApplication::overrideCursor())
            {
                QApplication::restoreOverrideCursor();
            }
            else
            {
                QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
            }
            _chatView->clear();
            _userListModel->clear();
            _mainView->setConnected(false);
            _mainView->setEnabled(true);
            _loginView->setFocus();
        }
        else if(state == Connecting)
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            _mainView->setEnabled(false);
        }
        else if(state == Connected)
        {
            QApplication::restoreOverrideCursor();
            _chatView->setEnabled(true);
            _mainView->setEnabled(true);
            _mainView->setConnected(true);
            _chatView->setFocus();
        }
        else if(state == Disconnecting)
        {
             QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            _mainView->setEnabled(false);
        }
    }

    virtual std::string
    getUsername() const
    {
        return _username;
    }

private:

    void destroySession()
    {
        if(_session)
        {
            setState(Disconnecting);
            _session->destroy();
        }
    }

    void destroySession(const Ice::Exception&)
    {
        _chatView->setEnabled(false);
        if(_session)
        {
            _session->destroy();
        }
    }

    void appendMessage(const std::string& message)
    {
        _chatView->appendMessage(message);
    }

    Glacier2::SessionHelperPtr _session;
    Glacier2::SessionFactoryHelperPtr _factory;

    MainView* _mainView;
    LoginView* _loginView;
    ChatView* _chatView;
    UserListModel* _userListModel;
    LoginInfoPtr _info;
    DispatcherPtr _dispatcher;
    ClientState _state;
    const unsigned int _maxMessageSize;
    std::string _username;
    bool _exit;
};

}

Application::Application(int argc, char** argv) :
    QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName("ZeroC");
    QCoreApplication::setOrganizationDomain("zeroc.com");
    QCoreApplication::setApplicationName("Chat Demo");

    LoginView* loginView = new LoginView(0);
    UserListModel* userListModel = new UserListModel();
    ChatView* chatView = new ChatView(0, userListModel);

    MainView* mainView = new MainView(loginView, chatView);

    _coordinator = new CoordinatorI(mainView, loginView, chatView, userListModel, argc, argv);

    setQuitOnLastWindowClosed(true);

    connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdown()));

    connect(mainView, SIGNAL(logout()), _coordinator.get(), SLOT(logout()));
    connect(mainView, SIGNAL(login()), _coordinator.get(), SLOT(login()));

    connect(loginView, SIGNAL(login(const LoginInfoPtr&)),
            _coordinator.get(), SLOT(login(const LoginInfoPtr&)));

    connect(chatView, SIGNAL(sendMessage(const std::string&)),
            _coordinator.get(), SLOT(sendMessage(const std::string&)));

    mainView->show();
}

void
Application::shutdown()
{
    _coordinator->exit();
}

int
main(int argc, char** argv)
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    Application app(argc, argv);
    app.setStyleSheet("QStatusBar::item{ border: 0px solid black; }");
    return app.exec();
}
