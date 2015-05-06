#pragma once

#include <dci/couple/generator/executor.hpp>
#include "out.hpp"

namespace dci { namespace couple { namespace generator { namespace impl
{

    class ExecutorCpp
        : public Executor
    {
    public:
        ExecutorCpp();
        virtual ~ExecutorCpp();

    protected:
        bool exec(
            const dci::couple::meta::Library &lib,
            const std::string &dir,
            const std::string &name) override = 0;

    protected:
        enum TypeNameFlags
        {
            inTarget = 1,
            inBody = 2,
            inWire = 4,
            instantiated = 32,
            ignoreTemplateTypename = 64,
            forGlobalScope = 128,
        };

        std::string typeName(const dci::couple::meta::Type *v, int flags);
        std::string typeName(const dci::couple::meta::Scope *v, const std::string &name, int flags);

        std::string methodArgiments(const dci::couple::meta::Method *v, bool forOutput, int typesFlags);
        std::string methodSignature(const dci::couple::meta::Method *m, int typesFlags, const dci::couple::meta::Iface *i=nullptr);

        const std::string &wireName() const;
        const std::string &bodyName() const;
        const std::string &runtimeNamespace() const;

    private:
        std::string _wireName;
        std::string _bodyName;

        const std::string _runtimeNamespace = "::dci::couple::runtime";
    };
}}}}
