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

        std::string methodArguments(const dci::couple::meta::Method *m, bool forRealMethod, int typesFlags);
        std::string methodArgumentsMove(const dci::couple::meta::Method *m);
        std::string methodReplyTypes(const dci::couple::meta::Method *m, int typesFlags);
        std::string methodName(const dci::couple::meta::Method *m, int typesFlags, const dci::couple::meta::Interface *i=nullptr);
        std::string methodSignature(const dci::couple::meta::Method *m, int typesFlags, const dci::couple::meta::Interface *i=nullptr);

        const std::string &wireName() const;
        const std::string &bodyName() const;
        const std::string &runtimeNamespace() const;
        const std::string &serializeNamespace() const;

        static std::string signInitializer(const runtime::Sign &sign);

        static std::vector<const dci::couple::meta::Interface *> interfaceWithAllBases(const dci::couple::meta::Interface *v);
        static void interfaceWithAllBases(std::vector<const dci::couple::meta::Interface *> &res, const dci::couple::meta::Interface *v);

    private:
        std::string _wireName;
        std::string _bodyName;

        const std::string _runtimeNamespace = "::dci::couple::runtime";
        const std::string _serializeNamespace = "::dci::couple::serialize";
    };
}}}}
