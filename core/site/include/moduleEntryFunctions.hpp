#pragma once

#include "modulePlace.hpp"
#include <dci/async/future.hpp>
#include <dci/couple/runtime/iface.hpp>

#include <system_error>


namespace dci { namespace site
{
    struct ModuleEntry
    {
        virtual async::Future<std::error_code> install(const dci::site::ModulePlace &place) = 0;
        virtual async::Future<std::error_code> uninstall(const dci::site::ModulePlace &place) = 0;

        virtual async::Future<std::error_code> load(const dci::site::ModulePlace &place) = 0;
        virtual async::Future<std::error_code> unload(const dci::site::ModulePlace &place) = 0;

        virtual async::Future<std::error_code> start(const dci::site::ModulePlace &place) = 0;
        virtual async::Future<std::error_code> stop(const dci::site::ModulePlace &place) = 0;

        virtual async::Future<std::error_code, couple::runtime::Iface> getServiceInstance(const couple::runtime::Iid &iid) = 0;
    };
}}
