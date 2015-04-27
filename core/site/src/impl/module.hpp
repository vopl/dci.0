#include <dci/couple/runtime/identifier.hpp>
#include <dci/couple/runtime/iface.hpp>
#include <moduleId.hpp>
#include "moduleState.hpp"
#include <modulePlace.hpp>
#include "modulePlace.hpp"
#include <moduleEntryFunctions.hpp>

#include <dci/async/future.hpp>

#include <string>
#include <vector>
#include <system_error>
#include <memory>

namespace dci { namespace site { namespace impl
{
    class Module
    {
    public:
        Module();
        ~Module();

        ////////////// identify
        const std::string &getProvider() const;

        const Mid &getId() const;
        const std::vector<couple::runtime::Iid> &getServieceIds() const;

        std::size_t getRevision() const;

        const std::string &getName() const;
        const std::string &getDescription() const;
        const std::vector<std::string> &getTags() const;

        /////////////////// dependencies
        const std::vector<couple::runtime::Iid> &getRequiredServiceIds() const;
        const std::vector<Mid> &getRequiredModuleIds() const;

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
        std::string                         _provider;
        Mid                                 _id;
        std::vector<couple::runtime::Iid>   _serviceIds;

        std::size_t                         _revision;
        std::string                         _name;
        std::string                         _description;
        std::vector<std::string>            _tags;

        std::vector<couple::runtime::Iid>   _requiredServiceIds;
        std::vector<Mid>                    _requiredModuleIds;

        std::string                         _mainBinary;

        ModuleState                         _state;
        ModulePlace                         _place;


    private:
        void *  _mainBinaryHandle;

    private:
        ModuleEntry * _entry;
    };

    using ModulePtr = std::unique_ptr<Module>;
}}}
