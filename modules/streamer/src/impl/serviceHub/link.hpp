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
    {
    protected:
        Link();
        ~Link();

    public:
        void setId(const streamer::ServiceHub::ServiceId &id);

    protected:
        streamer::ServiceHub::ServiceId _id;
//        Writer _writer;

//        void onInput(Msg1 msg1);
//        void onInput(Msg2 msg2);
//        void onInput(Msg3 msg3);
    };
}}
