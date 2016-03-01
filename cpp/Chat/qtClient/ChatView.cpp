// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatView.h>

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QSplitter>
#include <QTextEdit>
#include <QTextCursor>
#include <QListView>
#include <QScrollBar>
#include <QIcon>
#include <QCloseEvent>
#include <QSettings>

using namespace Chat;
using namespace std;

int
UserListModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(_users.size());
}

QVariant
UserListModel::data(const QModelIndex& index, int role) const
{
    QVariant data;
    if(index.isValid())
    {
        switch(role)
        {
            case Qt::DisplayRole:
            {
                data = _users[index.row()].c_str();
                break;
            }
            case Qt::DecorationRole:
            {
                data = QIcon(":/icons/user.png");
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return data;
}

QVariant
UserListModel::headerData(int, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return QVariant("users");
    }
    return QVariant();
}

void
UserListModel::addElement(const std::string& element)
{
    beginInsertRows(QModelIndex(), static_cast<int>(_users.size()), static_cast<int>(_users.size()));
    _users.push_back(element);
    endInsertRows();
}

void
UserListModel::removeElement(const std::string& element)
{
    Ice::StringSeq::iterator i = find(_users.begin(), _users.end(), element);
    assert(i != _users.end());
    int index = distance(_users.begin(), i);

    beginRemoveRows(QModelIndex(), index, index);
    _users.erase(i);
    endRemoveRows();
}

void
UserListModel::clear()
{
    if(_users.size() <= 0)
    {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, static_cast<int>(_users.size()) - 1);
    _users.clear();
    endRemoveRows();
}

bool
ChatView::eventFilter(QObject* target, QEvent* event)
{
    if(target == _messageView)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            assert(keyEvent);
            if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                QString text = _messageView->toPlainText();
                _messageView->setPlainText("");
                emit sendMessage(text.toUtf8().data());
                return true;
            }
        }
    }
    return QWidget::eventFilter(target, event);
}

ChatView::ChatView(QWidget* parent, UserListModel* userListModel) :
    QWidget(parent),
    _userListModel(userListModel),
    _usersView(new QListView()),
    _historyView(new QTextEdit()),
    _messageView(new QTextEdit())
{
    _usersView->setModel(_userListModel);
    _historyView->setReadOnly(true);

    QSplitter* s1 = new QSplitter();
    QSplitter* s2 = new QSplitter(Qt::Vertical, this);

    s1->addWidget(_usersView);
    s1->addWidget(_historyView);

    s2->addWidget(s1);
    s2->addWidget(_messageView);
    _messageView->installEventFilter(this);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(s2);
    setLayout(layout);

    int i = s1->indexOf(_usersView);
    s1->setStretchFactor(i, 1);
    i = s1->indexOf(_historyView);
    s1->setStretchFactor(i, 3);

    i = s2->indexOf(s1);
    s2->setStretchFactor(i, 5);
    i = s2->indexOf(_messageView);
    s2->setStretchFactor(i, 1);
}

void
ChatView::appendMessage(const std::string& message)
{
    _historyView->append(message.c_str());
    QScrollBar *sb = _historyView->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void
ChatView::clear()
{
    _historyView->setPlainText("");
    _messageView->setPlainText("");
}

void
ChatView::setFocus()
{
    _messageView->setFocus();
}
