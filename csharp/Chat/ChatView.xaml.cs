// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.ComponentModel;

namespace ChatDemoGUI
{
    //
    // This is a partial class that implements the logic for the ChatView.xaml
    // WPF Page.
    //
    public partial class ChatView : Page
    {
        public ChatView()
        {
            InitializeComponent();
            _coordinator = ((App)System.Windows.Application.Current).getCoordinator();
            _coordinator.setChatView(this);
        }

        public void appendMessage(string message)
        {
            txtMessages.AppendText(message);
            txtMessages.ScrollToEnd();
        }

        public void appendError(string error)
        {
            appendMessage(error);
            txtChatInputLine.IsReadOnly = true;
        }

        public void clearMessages()
        {
            txtMessages.Text = "";
        }

        public void setFocusToInput()
        {
            txtChatInputLine.IsReadOnly = false;
            Keyboard.Focus(txtChatInputLine);
        }

        //
        // Event handler attached to txtChatImputLine onKeyDown.
        // If the key is the Enter key, it sends the message asynchronously
        // and cleans the input line; otherwise, it does nothing.
        //
        private void sendMessage(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                string message = txtChatInputLine.Text.Trim();
                if(message.Length > 0)
                {
                    _coordinator.sendMessage(message);
                }
                txtChatInputLine.Text = "";
            }
        }

        private void scrollDown(object sender, SizeChangedEventArgs e)
        {
            txtMessages.ScrollToEnd();
        }

        private void pageLoaded(object sender, RoutedEventArgs e)
        {
            setFocusToInput();
        }

        private Coordinator _coordinator;
    }
}
