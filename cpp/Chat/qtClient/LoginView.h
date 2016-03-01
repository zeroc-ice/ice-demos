// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_LOGIN_VIEW_H
#define CHAT_LOGIN_VIEW_H

#include <Coordinator.h>
#include <QWidget>

class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QKeyEvent;

namespace Chat
{

class LoginView : public QWidget
{

Q_OBJECT

public:

    LoginView(QWidget*);
    bool eventFilter(QObject* target, QEvent* event);
    void setFocus();

private slots:

    void login();

signals:

    void login(const LoginInfoPtr&);
    void logout();

private:

    QLabel* _lblTitle;
    QLabel* _lblWelcome;

    QLabel* _lblUsername;
    QLineEdit* _lneUsername;
    QLabel* _lblPassword;
    QLineEdit* _lnePassword;
    QLabel* _lblHost;
    QCheckBox* _chkAvancedOptions;
    QLineEdit* _lneHost;
    QPushButton* _pbtLogin;
    LoginInfoPtr _info;
};

}
#endif
