#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/mm/newDelete.hpp>
#include "streamer.hpp"

namespace impl { namespace serviceHub
{

    using namespace dci::couple::runtime;
    using namespace streamer;

    //
    class Link
        : public dci::mm::NewDelete<Link>
    {
    public:
        void setId(const streamer::ServiceHub::ServiceId &id)
        {
            _id = id;
        }

        Future<ServiceHub::ServiceId> inject(streamer::ServiceHub::Remote &&arg_0)
        {
            (void)arg_0;
            //assert(0);

            return Future<ServiceHub::ServiceId>(make_error_code(std::errc::address_not_available));
        }

        Future<streamer::ServiceHub::Local, ServiceHub::ServiceId> invite(Iid &&arg_0)
        {
            (void)arg_0;
            assert(0);
            return Future<streamer::ServiceHub::Local, ServiceHub::ServiceId>(make_error_code(std::errc::address_not_available));
        }

        Future<streamer::ServiceHub::Remote> shutdown()
        {
            assert(0);
            return Future<streamer::ServiceHub::Remote>(make_error_code(std::errc::address_not_available));
        }

    private:
        streamer::ServiceHub::ServiceId _id;
//        Writer _writer;

//        void onInput(Msg1 msg1);
//        void onInput(Msg2 msg2);
//        void onInput(Msg3 msg3);
    };
}}
