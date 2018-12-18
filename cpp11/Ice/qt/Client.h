// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include <QtWidgets>
#include <memory>
#include <Ice/Ice.h>
#include <Hello.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow();
    virtual bool event(QEvent*);

public slots:

    void sayHello();
    void shutdown();
    void flush();
    void updateProxy();

private:

    QLineEdit* _hostname;
    QComboBox* _mode;
    QSlider* _timeout;
    QSlider* _delay;
    QPushButton* _hello;
    QPushButton* _shutdown;
    QPushButton* _flush;
    Ice::CommunicatorHolder _communicator;
    std::shared_ptr<Demo::HelloPrx> _helloPrx;
};

#endif
