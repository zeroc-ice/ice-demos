// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include "App.g.h"

namespace simpleChat
{
        /// <summary>
        /// Provides application-specific behavior to supplement the default Application class.
        /// </summary>
        ref class App sealed
        {
        public:
                App();
                virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs) override;

        private:
                void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
        };
}
