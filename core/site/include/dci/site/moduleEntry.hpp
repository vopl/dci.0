#pragma once

#include "modulePlace.hpp"
#include "moduleInfo.hpp"
#include "manager.hpp"
#include "serviceFactory.hpp"
#include <dci/async/future.hpp>
#include <dci/couple/runtime/interface.hpp>

#include <system_error>


namespace dci { namespace site
{
    struct ModuleEntry
    {
        virtual ~ModuleEntry() {}

        virtual const ModuleInfo &getInfo() = 0;

        virtual async::Future<std::error_code> install(const ModulePlace &place) = 0;
        virtual async::Future<std::error_code> uninstall(const ModulePlace &place) = 0;

        virtual async::Future<std::error_code> load(const ModulePlace &place) = 0;
        virtual async::Future<std::error_code> unload(const ModulePlace &place) = 0;

        virtual async::Future<std::error_code> start(Manager &manager, const ModulePlace &place) = 0;
        virtual async::Future<std::error_code> stop(const ModulePlace &place) = 0;

        virtual ServiceFactory *allocServiceFactory(const couple::runtime::Iid &iid) = 0;
        virtual void freeServiceFactory(const couple::runtime::Iid &iid, ServiceFactory *factory) = 0;
    };
}}
