#pragma once

#include <string>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Named
    {

    public:
        Named();
        ~Named();

        const std::string &name() const;
        void setName(const std::string &name);

    private:
        std::string _name;
    };

}}}}
