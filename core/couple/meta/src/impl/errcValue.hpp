#pragma once

#include "named.hpp"
#include "signed.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    class ErrcValue
        : public Named
        , public Signed
    {
    public:
        ErrcValue();
        ~ErrcValue();

        const std::string &description() const;
        void setDescription(const std::string &v);

        void makeSign();

    private:
        std::string _description{};
    };

}}}}
