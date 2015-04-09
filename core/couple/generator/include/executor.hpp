#pragma once

#include <string>
#include <map>
#include <system_error>
#include <dci/couple/meta/library.hpp>

namespace dci { namespace couple { namespace generator
{

    struct Executor
    {

        virtual std::string name() = 0;
        virtual std::string description() = 0;

        virtual std::error_code exec(
            const dci::couple::meta::Library &lib,
            const std::string &dir) = 0;


        static const std::map<std::string, Executor *> &getAll();
    };
}}}
