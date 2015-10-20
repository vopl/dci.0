#pragma once

#include "link.hpp"

namespace impl { namespace serviceHub
{

    //
    class LocalLink
        : public Link
        , public dci::mm::NewDelete<Link>
    {
    public:
        LocalLink();
        ~LocalLink();

        void stop();

        Future<ServiceHub::ServiceId> inject(streamer::ServiceHub::Remote &&arg_0);
        Future<streamer::ServiceHub::Local, ServiceHub::ServiceId> invite(Iid &&arg_0);
        Future<streamer::ServiceHub::Remote> eject();

    private:
    };
}}
