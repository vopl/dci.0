#pragma once

#include "sign.hpp"
#include <system_error>

std::error_code make_error_code(...) = delete;

namespace dci { namespace couple { namespace runtime
{
    template <class E>
    struct ErrcInitializer
    {
        static const std::error_code _initializer;

        static const std::error_code initialize()
        {
            return std::error_code();
        }
    };



    template <class E>
    const std::error_code ErrcInitializer<E>::_initializer = ErrcInitializer<E>::initialize();

//    template <class E>
//    const std::error_code ErrcInitializer<E>::initialize()
//    {
//        return ::make_error_code(static_cast<E>(0));
//    }

}}}
