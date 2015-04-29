#pragma once

#include <string>
#include <cstdint>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    class Sign
    {
    public:
        static const std::size_t _size = 16;

    public:
        Sign();
        Sign(const std::uint8_t (&data)[_size]);
        Sign(const Sign &from);
        Sign(Sign &&from);
        ~Sign();

        Sign &operator=(const Sign &from);
        Sign &operator=(Sign &&from);

        char *data();
        const char *data() const;

        std::string toHex(std::size_t chars=_size*2) const;
        bool fromHex(const std::string &txt);
        void fromRnd();

    private:
        std::uint8_t _data[_size];
    };

}}}}
