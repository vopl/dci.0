#pragma once

#include <executor.hpp>
#include "out.hpp"

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

        bool exec(
            const dci::couple::meta::Library &lib,
            const std::string &dir,
            const std::string &name) override;

    private:
        bool writeWire(const dci::couple::meta::Scope *v);
        bool writeWire(const dci::couple::meta::Struct *v);
        bool writeWire(const dci::couple::meta::Variant *v);
        bool writeWire(const dci::couple::meta::Iface *v);

    private:
        void writeBody(const dci::couple::meta::Scope *scope, bool withSelf);
        void writeBody(const dci::couple::meta::Struct *v);
        void writeBody(const dci::couple::meta::Variant *v);
        void writeBody(const dci::couple::meta::Enum *v);
        void writeBody(const dci::couple::meta::Alias *v);
        void writeBody(const dci::couple::meta::Iface *v);

    private:
        void writeIid(const dci::couple::meta::Scope *v);
        void writeIid(const dci::couple::meta::Iface *v);

    private:
        void writeTarget(const dci::couple::meta::Scope *scope);

        template <class T>
        void writeTarget(const T *v);

        void writeIfaceBody(const dci::couple::meta::Iface *v, bool opposite);

    private:
        enum TypeNameFlags
        {
            inBody = 0,
            inWire = 1,
            ignoreTemplateTypename = 4,
            forGlobalScope = 8,
            instantiated = 16,
        };

        std::string typeName(const dci::couple::meta::Type *v, int flags=0);
        std::string typeName(const dci::couple::meta::Scope *v, const std::string &name, int flags);

        std::string methodArgiments(const dci::couple::meta::Method *v, bool forOutput);
        std::string methodSignature(const dci::couple::meta::Method *m, const dci::couple::meta::Iface *i=nullptr);

    private:
        std::string _wireName;
        std::string _bodyName;

        //const std::string _idlNamespace = "::dci::couple::runtime::idl";
        const std::string _idlNamespace = "idl";

        Out _hpp;
    };
}}}}
