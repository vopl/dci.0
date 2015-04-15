#pragma once

#include <string>
#include <cstdint>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Sign
    {
    public:
        Sign();
        ~Sign();

        static const std::size_t _size = 32;
        char *data();
        const char *data() const;

        std::string string(std::size_t chars=_size*2) const;

    private:
        union Data
        {
            std::uint8_t    _by8[32];
            std::uint16_t   _by16[16];
            std::uint32_t   _by32[8];
            std::uint64_t   _by64[4];

            Data();
        } _data;
    };

}}}}
