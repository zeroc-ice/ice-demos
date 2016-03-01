// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace ChatDemoGUI
{
    public class ChatModel : INotifyPropertyChanged
    {
        public ChatModel()
        {
            _state = Coordinator.ClientState.Disconnected;
            _currentFrame = "LoginView.xaml";
            _loginInfo = new LoginInfo();
            _loginInfo.load();
        }

        //
        // This property stores the visible frame inside the
        // MainView. This can be LoginView.xaml or ChatView.xaml.
        //
        public string CurrentFrame
        {
            get
            {
                return _currentFrame;
            }
            set
            {
                if(value != _currentFrame)
                {
                    _currentFrame = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("CurrentFrame"));
                }
            }
        }

        public Coordinator.ClientState State
        {
            get
            {
                return _state;
            }
            set
            {
                if(_state != value)
                {
                    _state = value;
                    if(_state == Coordinator.ClientState.Disconnected)
                    {
                        CurrentFrame = "LoginView.xaml";
                    }
                    else if(_state == Coordinator.ClientState.Connected)
                    {
                        CurrentFrame = "ChatView.xaml";
                    }
                    OnPropertyChanged(new PropertyChangedEventArgs("State"));
                }
            }
        }

        //
        // Property to store information needed for login.
        //
        public LoginInfo LoginData
        {
            get
            {
                return _loginInfo;
            }
        }

        //
        // This property stores whether the UI should show advanced options.
        //
        public bool ShowAdvanced
        {
            get
            {
                return _showAdvanced;
            }
            set
            {
                _showAdvanced = value;
                OnPropertyChanged(new PropertyChangedEventArgs("ShowAdvanced"));
            }
        }


        #region INotifyPropertyChanged Members
        protected virtual void OnPropertyChanged(PropertyChangedEventArgs args)
        {
            if(PropertyChanged != null)
            {
                PropertyChanged(this, args);
            }
        }
        public event PropertyChangedEventHandler PropertyChanged;
        #endregion

        private Coordinator.ClientState _state = Coordinator.ClientState.Disconnected;
        private string _currentFrame;
        private LoginInfo _loginInfo = null;
        private bool _showAdvanced = false;
    }
}
