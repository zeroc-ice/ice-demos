// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_MAIN_VIEW_H
#define CHAT_MAIN_VIEW_H

#include <QMainWindow>

class QMenu;
class QAction;
class QLabel;
class QWidget;
class QStackedWidget;

namespace Chat
{

class MainView : public QMainWindow
{

Q_OBJECT

public:

    MainView(QWidget*, QWidget*);
    void storeWindowPrefs();
    void closeEvent(QCloseEvent*);
    void setConnected(bool);
    void enableLoginAction();

public slots:

    void about();

signals:

    void logout();
    void login();

private:

    // Menus
    QMenu* _fileMenu;
    QMenu* _helpMenu;

    //Actions
    QAction* _loginAction;
    QAction* _logoutAction;
    QAction* _exitAction;

    QAction* _aboutAction;

    QStackedWidget* _stackWidget;
    QLabel* _statusBar;

    QWidget* _loginView;
    QWidget* _chatView;
};

}

#endif
