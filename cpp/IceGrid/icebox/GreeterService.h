// Copyright (c) ZeroC, Inc.

#ifndef GREETER_SERVICE_H
#define GREETER_SERVICE_H

#include <Ice/Ice.h>
#include <IceBox/IceBox.h>

namespace Service
{
    /// An IceBox service that provides an implementation of the Greeter interface.
    class GreeterService final : public IceBox::Service
    {
    public:
        // See IceBox::Service::start doc-comment.
        void start(const std::string& name, const Ice::CommunicatorPtr& communicator, const Ice::StringSeq& args) final;

        // See IceBox::Service::stop doc-comment.
        void stop() final;

    private:
        // The name of the greeter.
        std::string _greeterName{"unknown"};

        // The object adapter created by this service.
        Ice::ObjectAdapterPtr _adapter;
    };
}

#endif
