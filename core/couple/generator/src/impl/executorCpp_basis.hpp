#pragma once

#include "executorCpp.hpp"
#include <set>

namespace dci { namespace couple { namespace generator { namespace impl
{

    class ExecutorCpp_basis
        : public ExecutorCpp
    {
    public:
        ExecutorCpp_basis();
        virtual ~ExecutorCpp_basis();

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
        bool writeWire(const dci::couple::meta::Interface *v);

        std::string interfaceWireTypeName(const dci::couple::meta::Interface *v);
        std::string interfaceWireMemberName(const dci::couple::meta::Interface *v, bool invertPrimary);

    private:
        void writeBody(const dci::couple::meta::Scope *scope, bool withSelf, bool aliasFromNext = false);
        void writeBody(const dci::couple::meta::Struct *v, bool aliasFromNext);
        void writeBody(const dci::couple::meta::Variant *v, bool aliasFromNext);
        void writeBody(const dci::couple::meta::Enum *v, bool aliasFromNext);
        void writeBody(const dci::couple::meta::Errc *v, bool aliasFromNext);
        void writeBody(const dci::couple::meta::Alias *v, bool aliasFromNext);
        void writeBody(const dci::couple::meta::Interface *v, bool aliasFromNext);

    private:
        void writeIid(const dci::couple::meta::Scope *v);
        void writeIid(const dci::couple::meta::Interface *v);

    private:
        template <class T>
        void writeTarget(const T *v);

        enum class Stage
        {
            pre,
            post
        };

        void writeTarget(const dci::couple::meta::Scope *v, Stage stage);

    private:
        void writeErrcSpares(const dci::couple::meta::Scope *scope);
        void writeErrcSpares(const dci::couple::meta::Errc *v);

    private:
        Out _hpp;
    };
}}}}
