#pragma once

#include "signed.hpp"
#include "container1.hpp"

#include <cstdint>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Array
        : public Signed
        , public Container1
    {
    public:
        Array();
        ~Array();

        void setSize(std::uint32_t v);
        std::uint32_t size() const;
        void makeSign();

    private:
        std::uint32_t _size{0};
    };

}}}}
