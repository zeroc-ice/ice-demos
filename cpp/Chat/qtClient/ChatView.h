// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_CHAT_VIEW_H
#define CHAT_CHAT_VIEW_H

#include <vector>
#include <string>

#include <QAbstractListModel>
#include <QWidget>

#include <Coordinator.h>

class QModelIndex;
class QListView;
class QTextEdit;

namespace Chat
{

class UserListModel : public QAbstractListModel
{

public:

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex&, int) const;
    virtual QVariant headerData(int, Qt::Orientation, int) const;

    void addElement(const std::string&);
    void removeElement(const std::string&);
    void clear();

private:

    std::vector<std::string> _users;
};

class ChatView : public QWidget
{

Q_OBJECT

public:

    ChatView(QWidget*, UserListModel*);
    bool eventFilter(QObject*, QEvent*);
    void appendMessage(const std::string&);
    void clear();
    void setFocus();

signals:

    void sendMessage(const std::string&);

private:

    UserListModel* _userListModel;
    QListView* _usersView;
    QTextEdit* _historyView;
    QTextEdit* _messageView;
};

}
#endif
