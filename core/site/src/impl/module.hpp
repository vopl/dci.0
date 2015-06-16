#pragma once

#include <dci/couple/runtime/interface.hpp>
#include "moduleState.hpp"
#include <dci/site/modulePlace.hpp>
#include "modulePlace.hpp"
#include <dci/site/moduleEntry.hpp>

#include <dci/async/future.hpp>

#include <system_error>
#include <memory>

namespace dci { namespace site { namespace impl
{
    class Manager;

    class Module
    {
    public:
        static std::string generateManifest(const std::string &mainBinaryFullPath);

    public:
        Module(Manager *site);
        ~Module();

        ////////////// identify
        const ModuleInfo &getInfo() const;

        ModuleState getState() const;

        /////////////// attach
        std::error_code attach(const ModulePlace &place);
        std::error_code detach();

        /////////////// install
        async::Future<std::error_code> install(const ModulePlace &place);
        async::Future<std::error_code> uninstall();

        /////////////// load
        async::Future<std::error_code> load();
        async::Future<std::error_code> unload();

        /////////////// run
        async::Future<std::error_code> start();
        async::Future<std::error_code> stop();




        //////////////// use
        ServiceFactory *allocServiceFactory(const couple::runtime::Iid &iid);
        void freeServiceFactory(const couple::runtime::Iid &iid, ServiceFactory *factory);

    private:
        Manager *       _manager;
        std::string     _mainBinary;

        ModuleState     _state;
        ModulePlace     _place;

    private:
        void *          _mainBinaryHandle;

        ModuleEntry *   _entry;
        ModuleInfo      _info;
    };

    using ModulePtr = std::unique_ptr<Module>;
}}}
