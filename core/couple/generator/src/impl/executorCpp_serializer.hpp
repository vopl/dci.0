#pragma once

#include "executorCpp.hpp"

namespace dci { namespace couple { namespace generator { namespace impl
{

    class ExecutorCpp_serializer
        : public ExecutorCpp
    {
    public:
        ExecutorCpp_serializer();
        virtual ~ExecutorCpp_serializer();

        std::string name() override;
        std::string description() override;

        bool exec(
            const dci::couple::meta::Library &lib,
            const std::string &dir,
            const std::string &name) override;

    private:
        bool writeBody(const dci::couple::meta::Scope *scope, bool withSelf);
        bool writeBody(const dci::couple::meta::Interface *v);

        std::string methodIdName(const dci::couple::meta::Interface *i, const dci::couple::meta::Method *m);

    private:
        Out _hpp;
    };
}}}}
