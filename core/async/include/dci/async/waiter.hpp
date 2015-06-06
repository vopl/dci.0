#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include <dci/mm/intrusiveDlist.hpp>

#include "api.hpp"

namespace dci { namespace async
{
    namespace impl
    {
        class Waiter;
        class Waitable;
    }

    struct WWLink
        : mm::IntrusiveDlistElement<WWLink>
    {
        impl::Waiter *      _waiter;
        impl::Waitable *    _waitable;
    };

    class APIDCI_ASYNC Waiter
        : public himpl::FaceLayout<impl::Waiter>
    {

    public:
        Waiter(WWLink *links, std::size_t amount);
        ~Waiter();

        std::size_t any();
        void all();
        void each();
    };

}}
