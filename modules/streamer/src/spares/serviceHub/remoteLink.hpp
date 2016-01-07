#pragma once

#include "link.hpp"

namespace spares { namespace serviceHub
{

    //
    class RemoteLink
        : public Link
        , public dci::mm::NewDelete<Link>
    {
    public:
        RemoteLink();
        ~RemoteLink();

        void stop();
        Future<streamer::ServiceHub::Remote> eject();

    private:
    };
}}
