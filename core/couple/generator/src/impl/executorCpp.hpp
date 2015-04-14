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
        bool writeWires(const dci::couple::meta::Scope *v);
        bool writeWires(const dci::couple::meta::Struct *v);
        bool writeWires(const dci::couple::meta::Variant *v);
        bool writeWires(const dci::couple::meta::Iface *v);

    private:
        void writeBodies(const dci::couple::meta::Scope *scope);
        void writeBodies(const dci::couple::meta::Struct *v);
        void writeBodies(const dci::couple::meta::Variant *v);
        void writeBodies(const dci::couple::meta::Enum *v);
        void writeBodies(const dci::couple::meta::Alias *v);
        void writeBodies(const dci::couple::meta::Iface *v);

    private:
        void writeTargets(const dci::couple::meta::Scope *scope);
        void writeTargets(const dci::couple::meta::Struct *v);
        void writeTargets(const dci::couple::meta::Variant *v);
        void writeTargets(const dci::couple::meta::Enum *v);
        void writeTargets(const dci::couple::meta::Alias *v);
        void writeTargets(const dci::couple::meta::Iface *v);

    private:
        std::string typeName(const dci::couple::meta::Type *v);
        std::string typeNameInBody(const dci::couple::meta::Scope *v, const std::string &name);

    private:
        std::string _wireName;
        std::string _bodyName;

        //const std::string _idlNamespace = "::dci::couple::runtime::idl";
        const std::string _idlNamespace = "idl";

        Out _hpp;
        Out _cpp;

    };
}}}}
