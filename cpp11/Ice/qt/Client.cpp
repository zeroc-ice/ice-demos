// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Client.h>

namespace
{

const int CUSTOM_EVENT_TYPE = QEvent::User + 1000;

enum DeliveryMode
{
    Twoway,
    TwowaySecure,
    Oneway,
    OnewayBatch,
    OnewaySecure,
    OnewaySecureBatch,
    Datagram,
    DatagramBatch
};

class DispatchEvent : public QEvent
{
public:

    DispatchEvent(std::function<void()> call) :
        QEvent(QEvent::Type(CUSTOM_EVENT_TYPE)),
        _call(call)
    {
    }

    void dispatch()
    {
        _call();
    }

private:

    std::function<void()> _call;
};

}

MainWindow::MainWindow() :
    _hostname(new QLineEdit("127.0.0.1")),
    _mode(new QComboBox()),
    _timeout(new QSlider(Qt::Horizontal)),
    _delay(new QSlider(Qt::Horizontal)),
    _hello(new QPushButton("Hello World!")),
    _shutdown(new QPushButton("Shutdown")),
    _flush(new QPushButton("flush"))
{
    auto layout = new QVBoxLayout();
    auto formLayout = new QFormLayout;

    formLayout->addRow("Hostname:", _hostname);

    _mode->addItem("Twoway");
    _mode->addItem("Twoway Secure");
    _mode->addItem("Oneway");
    _mode->addItem("Oneway Batch");
    _mode->addItem("Oneway Secure");
    _mode->addItem("Oneway Secure Batch");
    _mode->addItem("Datagram");
    _mode->addItem("Datagram Batch");

    formLayout->addRow("Mode:", _mode);

    _timeout->setRange(0, 5000);
    formLayout->addRow("Timeout:", _timeout);

    _delay->setRange(0, 5000);
    formLayout->addRow("Delay:", _delay);

    layout->addLayout(formLayout);

    _flush->setEnabled(false);
    {
        auto h = new QHBoxLayout();
        h->addWidget(_hello);
        h->addWidget(_shutdown);
        h->addWidget(_flush);
        layout->addLayout(h);
    }

    connect(_hello, &QPushButton::clicked, this, &MainWindow::sayHello);
    connect(_shutdown, &QPushButton::clicked, this, &MainWindow::shutdown);
    connect(_flush, &QPushButton::clicked, this, &MainWindow::flush);

    connect(_hostname, &QLineEdit::textChanged, this, &MainWindow::updateProxy);
    connect(_timeout, &QSlider::valueChanged, this, &MainWindow::updateProxy);
    connect(_mode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::updateProxy);

    auto centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    Ice::InitializationData initData;
    initData.properties = Ice::createProperties();
    initData.properties->load("config.client");
    initData.dispatcher = [this](std::function<void()> dispatchCall, const std::shared_ptr<Ice::Connection>&)
        {
            QApplication::postEvent(this, new DispatchEvent(dispatchCall));
        };
    _communicator = Ice::initialize(initData);
    updateProxy();
}

bool
MainWindow::event(QEvent* event)
{
    if(event->type() == CUSTOM_EVENT_TYPE)
    {
        auto dispatchEvent = static_cast<DispatchEvent*>(event);
        try
        {
            dispatchEvent->dispatch();
        }
        catch(const Ice::Exception& ex)
        {
            statusBar()->showMessage(ex.ice_id().c_str());
        }
        catch(const std::exception& ex)
        {
            QMessageBox error(QMessageBox::Critical, "Error", ex.what());
            error.exec();
            QApplication::exit(EXIT_FAILURE);
        }
        return true;
    }
    return QMainWindow::event(event);
}

void
MainWindow::updateProxy()
{
    auto host = _hostname->text().trimmed();
    if(host.isEmpty())
    {
        statusBar()->showMessage("No hostname");
        return;
    }

    auto s = QString("hello:tcp -h %1 -p 10000:ssl -h %1 -p 10001:udp -h %1 -p 10000").arg(host);
    auto prx = _communicator->stringToProxy(s.toStdString());
    switch(_mode->currentIndex())
    {
        case Twoway:
        {
            prx = prx->ice_twoway();
            break;
        }
        case TwowaySecure:
        {
            prx = prx->ice_twoway()->ice_secure(true);
            break;
        }
        case Oneway:
        {
            prx = prx->ice_oneway();
            break;
        }
        case OnewayBatch:
        {
            prx = prx->ice_batchOneway();
            break;
        }
        case OnewaySecure:
        {
            prx = prx->ice_oneway()->ice_secure(true);
            break;
        }
        case OnewaySecureBatch:
        {
            prx = prx->ice_batchOneway()->ice_secure(true);
            break;
        }
        case Datagram:
        {
            prx = prx->ice_datagram();
            break;
        }
        case DatagramBatch:
        {
            prx = prx->ice_batchDatagram();
            break;
        }
    }
    int timeout = _timeout->value();
    if(timeout != 0)
    {
        prx = prx->ice_invocationTimeout(timeout);
    }
    _helloPrx = Ice::uncheckedCast<Demo::HelloPrx>(prx);

    //
    // The batch requests associated to the proxy are lost when we
    // update the proxy.
    //
    _flush->setEnabled(false);

    statusBar()->showMessage("Ready");
}

void
MainWindow::sayHello()
{
    if(_helloPrx)
    {
        int delay = _delay->value();
        try
        {
            DeliveryMode mode = static_cast<DeliveryMode>(_mode->currentIndex());
            if(mode == OnewayBatch || mode == OnewaySecureBatch || mode == DatagramBatch)
            {
                _flush->setEnabled(true);
                _helloPrx->sayHello(delay);
                statusBar()->showMessage("Queued sayHello request");
            }
            else
            {
                statusBar()->showMessage("Sending request");
                _helloPrx->sayHelloAsync(delay,
                    [this]()
                    {
                        statusBar()->showMessage("Ready");
                    },
                    [this](std::exception_ptr errptr)
                    {
                        try
                        {
                            std::rethrow_exception(errptr);
                        }
                        catch(const Ice::Exception& ex)
                        {
                            statusBar()->showMessage(ex.ice_id().c_str());
                        }
                        catch(const std::exception& ex)
                        {
                            QMessageBox error(QMessageBox::Critical, "Error", ex.what());
                            error.exec();
                            QApplication::exit(EXIT_FAILURE);
                        }
                    },
                    [this, mode](bool)
                    {
                        if(mode == Oneway || mode == OnewaySecure || mode == Datagram)
                        {
                            statusBar()->showMessage("Ready");
                        }
                        else
                        {
                            statusBar()->showMessage("Waiting for response");
                        }
                    });
            }
        }
        catch(const Ice::Exception& ex)
        {
            statusBar()->showMessage(ex.ice_id().c_str());
        }
        catch(const std::exception& ex)
        {
            QMessageBox error(QMessageBox::Critical, "Error", ex.what());
            error.exec();
            QApplication::exit(EXIT_FAILURE);
        }
    }
}

void
MainWindow::shutdown()
{
    if(_helloPrx)
    {
        try
        {
            DeliveryMode mode = static_cast<DeliveryMode>(_mode->currentIndex());
            if(mode == OnewayBatch || mode == OnewaySecureBatch || mode == DatagramBatch)
            {
                _flush->setEnabled(true);
                _helloPrx->shutdown();
                statusBar()->showMessage("Queued sayHello request");
            }
            else
            {
                statusBar()->showMessage("Sending request");
                _helloPrx->shutdownAsync(
                    [this]()
                        {
                            statusBar()->showMessage("Ready");
                        },
                    [this](std::exception_ptr errptr)
                        {
                            try
                            {
                                std::rethrow_exception(errptr);
                            }
                            catch(const Ice::Exception& ex)
                            {
                                statusBar()->showMessage(ex.ice_id().c_str());
                            }
                            catch(const std::exception& ex)
                            {
                                QMessageBox error(QMessageBox::Critical, "Error", ex.what());
                                error.exec();
                                QApplication::exit(EXIT_FAILURE);
                            }
                        },
                    [this, mode](bool)
                        {
                            if (mode == Oneway || mode == OnewaySecure || mode == Datagram)
                            {
                                statusBar()->showMessage("Ready");
                            }
                            else
                            {
                                statusBar()->showMessage("Waiting for response");
                            }
                        });
            }
        }
        catch(const Ice::Exception& ex)
        {
            statusBar()->showMessage(ex.ice_id().c_str());
        }
        catch(const std::exception& ex)
        {
            QMessageBox error(QMessageBox::Critical, "Error", ex.what());
            error.exec();
            QApplication::exit(EXIT_FAILURE);
        }
    }
}

void
MainWindow::flush()
{
    if(_helloPrx)
    {
        _helloPrx->ice_flushBatchRequestsAsync([this](std::exception_ptr errptr)
                                                    {
                                                        try
                                                        {
                                                            std::rethrow_exception(errptr);
                                                        }
                                                        catch(const Ice::Exception& ex)
                                                        {
                                                            statusBar()->showMessage(ex.ice_id().c_str());
                                                        }
                                                        catch(const std::exception& ex)
                                                        {
                                                            QMessageBox error(QMessageBox::Critical, "Error", ex.what());
                                                            error.exec();
                                                            QApplication::exit(EXIT_FAILURE);
                                                        }
                                                    });
        _flush->setEnabled(false);
        statusBar()->showMessage("Flushed batch requests");
    }
}

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    catch(const std::exception& ex)
    {
        QMessageBox error(QMessageBox::Critical, "Initialization exception", ex.what());
        error.exec();
        return EXIT_FAILURE;
    }
}
