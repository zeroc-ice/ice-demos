// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <LoginView.h>

#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QSettings>

using namespace std;
using namespace Chat;

LoginView::LoginView(QWidget* parent) :
    QWidget(parent),
    _info(new LoginInfo())
{
    _info->load();
    setObjectName("loginForm");

    _lblTitle = new QLabel(this);
    _lblTitle->setText(tr("Chat Demo"));

    _lblWelcome = new QLabel(this);
    _lblWelcome->setText(tr("Welcome! You can login with any username / password."));

    _lblUsername = new QLabel(this);
    _lblUsername->setText(tr("Username"));

    _lneUsername = new QLineEdit(this);
    _lneUsername->setText(_info->username().c_str());
    _lneUsername->installEventFilter(this);

    _lblPassword = new QLabel(this);
    _lblPassword->setText(tr("Password"));

    _lnePassword = new QLineEdit(this);
    _lnePassword->setEchoMode(QLineEdit::Password);
    _lnePassword->setText(_info->password().c_str());
    _lnePassword->installEventFilter(this);

    _pbtLogin = new QPushButton(this);
    _pbtLogin->setText(tr("Login"));
    _pbtLogin->setDefault(true);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addStretch();

    QHBoxLayout* hLayout;

    //
    // Username
    //
    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_lblTitle);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_lblWelcome);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);

    //
    // Username
    //
    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_lblUsername);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_lneUsername);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);

    //
    // Password
    //
    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_lblPassword);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_lnePassword);
    hLayout->addStretch();
    vLayout->addLayout(hLayout);


    //
    // Add login button
    //
    hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(_pbtLogin);
    hLayout->addStretch();

    vLayout->addLayout(hLayout);
    vLayout->addStretch();
    vLayout->setObjectName("LoginViewLayout");
    setLayout(vLayout);

    connect(_pbtLogin, SIGNAL(clicked()), this, SLOT(login()));
}

void
LoginView::setFocus()
{
    _lneUsername->setFocus();
}

void
LoginView::login()
{
    _info->setUsername(_lneUsername->text().toUtf8().data());
    _info->setPassword(_lnePassword->text().toUtf8().data());
    emit login(_info);
}

bool
LoginView::eventFilter(QObject* target, QEvent* event)
{
    if(target == _lneUsername || target == _lnePassword)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            assert(keyEvent);
            if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                if(target == _lneUsername)
                {
                    _lnePassword->setFocus();
                }
                else if(target == _lnePassword)
                {
                    _pbtLogin->setFocus();
                }
                return true;
            }
        }
    }
    return QWidget::eventFilter(target, event);
}
