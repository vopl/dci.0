#pragma once

#include "executorCpp.hpp"

namespace dci { namespace couple { namespace generator { namespace impl
{

    class ExecutorCpp_handlerSkel
        : public ExecutorCpp
    {
    public:
        ExecutorCpp_handlerSkel();
        virtual ~ExecutorCpp_handlerSkel();

        std::string name() override;
        std::string description() override;

        bool exec(
            const dci::couple::meta::Library &lib,
            const std::string &dir,
            const std::string &name) override;

    private:
        bool writeBody(const dci::couple::meta::Scope *scope, bool withSelf);
        bool writeBody(const dci::couple::meta::Interface *v);

    private:
        Out _hpp;
    };
}}}}
