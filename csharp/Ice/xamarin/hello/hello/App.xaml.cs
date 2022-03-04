//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

[assembly: XamlCompilation (XamlCompilationOptions.Compile)]
namespace hello
{
    public partial class App : Application
    {
        public App (string defaultHost = "127.0.0.1")
        {
            InitializeComponent();

            MainPage = new MainPage(defaultHost);
        }

        protected override void OnStart ()
        {
            // Handle when your app starts
        }

        protected override void OnSleep ()
        {
            // Handle when your app sleeps
        }

        protected override void OnResume ()
        {
            // Handle when your app resumes
        }
    }
}
