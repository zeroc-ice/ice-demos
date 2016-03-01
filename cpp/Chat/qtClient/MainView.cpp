// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <iostream>
#include <iomanip>

#include <Ice/Ice.h>

#include <Chat.h>
#include <ChatSession.h>

#include <QAction>
#include <QSettings>
#include <QSize>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QLabel>
#include <QStackedWidget>
#include <QStatusBar>
#include <QCloseEvent>

#include <MainView.h>
#include <Util.h>

using namespace std;
using namespace Chat;

MainView::MainView(QWidget* loginView, QWidget* chatView) :
    QMainWindow(0),
    _loginView(loginView),
    _chatView(chatView)
{
    setWindowTitle(tr("Chat Demo"));
    setWindowIcon(QIcon(":/icons/logo.png"));

    _stackWidget = new QStackedWidget();
    _stackWidget->addWidget(loginView);
    _stackWidget->addWidget(chatView);
    setCentralWidget(_stackWidget);

    _statusBar = new QLabel();
    statusBar()->addPermanentWidget(_statusBar);

    _loginAction = new QAction(tr("Login"), this);
    connect(_loginAction, SIGNAL(triggered()), this, SIGNAL(login()));

    _logoutAction = new QAction(tr("Logout"), this);
    connect(_logoutAction, SIGNAL(triggered()), this, SIGNAL(logout()));

    _exitAction = new QAction(tr("Exit"), this);
    connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    _aboutAction = new QAction(tr("About"), this);
    connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    //
    // File menu
    //
    _fileMenu = menuBar()->addMenu(tr("&File"));
    _fileMenu->addAction(_loginAction);
    _fileMenu->addAction(_logoutAction);
    _fileMenu->addAction(_exitAction);

    //
    // Help menu
    //
    _helpMenu = menuBar()->addMenu(tr("&Help"));
    _helpMenu->addAction(_aboutAction);

    //
    // Restore window geometry.
    //
    QSettings settings("ZeroC", "Chat Demo");
    settings.beginGroup("MainView");
    resize(settings.value("size", QSize(400, 540)).toSize());
    if(settings.contains("position"))
    {
        move(settings.value("position").toPoint());
    }
    else
    {
        centerWidgetOnScreen(this);
    }
    settings.endGroup();
}

void
MainView::storeWindowPrefs()
{
    QSettings settings("ZeroC", "Chat Demo");
    settings.beginGroup("MainView");
    settings.setValue("size", size());
    settings.setValue("position", pos());
    settings.endGroup();
    settings.sync();
}

void
MainView::closeEvent(QCloseEvent* event)
{
    storeWindowPrefs();
    event->accept();
}

void
MainView::enableLoginAction()
{
    _loginAction->setEnabled(true);
    _logoutAction->setEnabled(false);
}

void
MainView::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowModality(Qt::WindowModal);
    QString message("Chat Demo.\nCopyright %1 2013 ZeroC, Inc. All rights reserved.\n");
    msgBox.setWindowTitle("Chat Demo - About");
    msgBox.setText(message.arg(QChar(0x00A9)));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void
MainView::setConnected(bool connected)
{
    if(connected)
    {
        _stackWidget->setCurrentWidget(_chatView);
        _statusBar->setText("Online");
    }
    else
    {
        _stackWidget->setCurrentWidget(_loginView);
        _statusBar->setText("Offline");
    }
    _loginAction->setEnabled(false);
    _logoutAction->setEnabled(connected);
}
