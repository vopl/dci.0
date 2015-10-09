#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/async.hpp>
#include "streamer.hpp"
#include "endpoint.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;


    class Accord
    {
    public:
        Accord();
        ~Accord();

    public:
        Future<> start();
        Future<> stop();

        template <class Message>
        Future<> write(Message &&msg);

        template <class F>
        void read(F &f);

    };
}
