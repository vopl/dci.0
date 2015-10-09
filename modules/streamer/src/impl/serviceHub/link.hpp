#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"
#include "../links/local/linkId.hpp"

namespace impl { namespace serviceHub
{

    using namespace dci::couple::runtime;
    using namespace streamer;

    //
    class Link
    {
    public:
        void setId(const streamer::ServiceHub::ServiceId &id)
        {
            _id = id;
        }

        void setId(const ::impl::links::local::LinkId &id)
        {
            assert(0);
            (void)id;
            //_id = id;
        }

        Future<ServiceHub::ServiceId> inject(streamer::ServiceHub::Remote &&arg_0)
        {
            (void)arg_0;
            assert(0);
        }

        Future<streamer::ServiceHub::Local, ServiceHub::ServiceId> invite(Iid &&arg_0)
        {
            (void)arg_0;
            assert(0);
        }

        Future<streamer::ServiceHub::Remote> shutdown()
        {
            assert(0);
        }

    private:
        streamer::ServiceHub::ServiceId _id;
//        Writer _writer;

//        void onInput(Msg1 msg1);
//        void onInput(Msg2 msg2);
//        void onInput(Msg3 msg3);
    };
}}
