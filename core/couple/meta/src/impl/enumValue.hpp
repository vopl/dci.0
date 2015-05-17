#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class EnumValue
        : public Named
        , public Signed
    {
    public:
        EnumValue();
        ~EnumValue();

        const std::string &value() const;
        void setValue(const std::string &v);

        void makeSign();

    private:
        std::string _value{};
    };

}}}}
