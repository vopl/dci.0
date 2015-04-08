#pragma once
#include <dci/couple/runtime/idl.hpp>

namespace scope
{
    namespace talk
    {
        struct Peer
        {
            using Id = ::dci::couple::runtime::idl::array< ::dci::couple::runtime::idl::int8, 32>;

            enum class Status
            {
                available=0,
                busy=1,
            };

        protected:
            template <class... T>
            using Future = ::dci::couple::runtime::idl::Future<T...>;

            template <class T>
            using ValuePorter = ::dci::couple::runtime::idl::ValuePorter<T>;

            template <class F>
            using Signal = ::dci::couple::runtime::idl::Signal<F>;
        };
    }
}
