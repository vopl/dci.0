#pragma once

#include <dci/couple/runtime/identifier.hpp>
#include <dci/couple/runtime/iface.hpp>
#include "moduleState.hpp"
#include <modulePlace.hpp>
#include "modulePlace.hpp"
#include <moduleEntry.hpp>

#include <dci/async/future.hpp>

#include <system_error>
#include <memory>

namespace dci { namespace site { namespace impl
{
    class Module
    {
    public:
        static std::string generateManifest(const std::string &mainBinaryFullPath);

    public:
        Module();
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
        async::Future<std::error_code, couple::runtime::Iface> getServiceInstance(const couple::runtime::Iid &iid);

    private:

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
