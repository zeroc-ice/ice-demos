// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;


namespace ChatDemoGUI
{
    // This class models the users of a chat room.
    class User
    {
        public User(string name)
        {
            _name = name;
        }

        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                _name = value;
            }
        }

        public override int GetHashCode()
        {
            return -1; // Not needed
        }

        public override bool Equals(object obj)
        {
            if(obj == null)
            {
                return false;
            }

            if(this.GetType() != obj.GetType())
            {
                return false;
            }
            User user = (User)obj;
            return user.Name == Name;
        }

        private string _name;
    }

    //
    // Extends ObservableCollection to store the list of chat users.
    //
    // We need to use ObservableCollection so the view is updated when the list contents change.
    //
    // Also note that updates to the list need to be done from the UI thread
    // for ObservableCollection to work correctly.
    //
    class UserList : ObservableCollection<User>
    {
        public UserList()
        {
        }
    }
}
