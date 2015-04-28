#pragma once

#include <dci/couple/runtime/ifaceId.hpp>
#include "moduleId.hpp"
#include <string>
#include <vector>

namespace dci { namespace site
{
    struct ModuleInfo
    {
        std::string                         _provider;
        Mid                                 _id;
        std::vector<couple::runtime::Iid>   _serviceIds;

        std::size_t                         _revision;
        std::string                         _name;
        std::string                         _description;
        std::vector<std::string>            _tags;

        std::vector<couple::runtime::Iid>   _requiredServiceIds;
        std::vector<Mid>                    _requiredModuleIds;

        ModuleInfo() = default;
    };
}}
