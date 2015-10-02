#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/async.hpp>
#include "streamer.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    //////////////////////////////////////////////////////////////////
    //events

    //general
    struct attach {};
    struct detach {};

    struct start {};
    struct stop {};
    struct reset {};


    struct null {};
    struct unknown {};

    //channel
    struct hello {};
    struct bye {};

    //links
    struct links_create {};
    struct links_destroy {};
    struct links_generation {};
    struct links_commitGeneration {};


    //link
    struct link_request {};
    struct link_response {};
    struct link_message {};

    //serviceHub
    struct link_inject {};
    struct link_invite {};
    struct link_eject {};

    //amortisseur
    struct recovery {};



    namespace
    {
        template <class F>
        struct ScopedCleaner
        {
            F _f;

            ScopedCleaner(F &&from)
                : _f(std::forward<F>(from))
            {
            }

            ScopedCleaner(ScopedCleaner &&from)
                : _f(std::move(from._f))
            {
            }

            ScopedCleaner &operator=(ScopedCleaner &&from)
            {
                _f = (std::move(from._f));
            }

            ~ScopedCleaner()
            {
                _f();
            }
        };

        template <class F>
        ScopedCleaner<F> makeScopedCleaner(F &&f)
        {
            return ScopedCleaner<F>(std::move(f));
        }
    }



    template <class Derived>
    class Endpoint
    {
    public:
        Endpoint();
        ~Endpoint();

    public:
        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

        //FSM _fsm;
    };
}
