// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_COORDINATOR_H
#define CHAT_COORDINATOR_H

#include <string>
#include <vector>

#include <IceUtil/IceUtil.h>
#include <IceSSL/IceSSL.h>

#include <Glacier2/Glacier2.h>
#include <Glacier2/SessionHelper.h>

#include <QApplication>

namespace Chat
{

const int CUSTOM_EVENT_TYPE = QEvent::User + 1000;

enum ClientState
{
    Disconnected,
    Connecting,
    Connected,
    Disconnecting
};

class LoginInfo : public IceUtil::Shared
{

public:

    std::string username() const;
    void setUsername(const std::string& username);

    std::string password() const;
    void setPassword(const std::string& password);

    std::string host() const;
    void setHost(const std::string& host);

    void load();
    void save();

private:

    IceUtil::Mutex _mutex;
    std::string _username;
    std::string _password;
    std::string _host;
};
typedef IceUtil::Handle<LoginInfo> LoginInfoPtr;

class Coordinator : public QObject, public Glacier2::SessionCallback
{

Q_OBJECT

public:

    bool event(QEvent*);

    virtual void exit() = 0;

    virtual void initEvent(const std::vector<std::string>&) = 0;
    virtual void userJoinEvent(Ice::Long, const std::string&) = 0;
    virtual void userLeaveEvent(Ice::Long, const std::string&) = 0;
    virtual void userSayEvent(Ice::Long, const std::string&, const std::string&) = 0;

    virtual void setState(ClientState) = 0;
    virtual std::string getUsername() const = 0;

    virtual void createdCommunicator(const Glacier2::SessionHelperPtr&) = 0;
    virtual void connected(const Glacier2::SessionHelperPtr&) = 0;
    virtual void disconnected(const Glacier2::SessionHelperPtr&) = 0;
    virtual void disconnected(const Glacier2::SessionHelperPtr&, const Ice::Exception&) = 0;
    virtual void connectFailed(const Glacier2::SessionHelperPtr&, const Ice::Exception&) = 0;

public slots:

    virtual void sendMessage(const std::string&) = 0;
    virtual void login(const LoginInfoPtr& info) = 0;
    virtual void login() = 0;
    virtual void logout() = 0;
    virtual void setError(const std::string&) = 0;
};
typedef IceUtil::Handle<Coordinator> CoordinatorPtr;

class Application : public QApplication
{

Q_OBJECT

public:

    Application(int argc, char** argv);

public slots:

    void shutdown();

private:

    CoordinatorPtr _coordinator;
};

}

#endif
