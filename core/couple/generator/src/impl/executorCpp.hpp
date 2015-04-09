#pragma once

#include <executor.hpp>

namespace dci { namespace couple { namespace generator { namespace impl
{

    class ExecutorCpp
        : public Executor
    {
    public:
        ExecutorCpp();
        virtual ~ExecutorCpp();

        std::string name() override;
        std::string description() override;

        std::error_code exec(
            const dci::couple::meta::Library &lib,
            const std::string &dir) override;

    };
}}}}
