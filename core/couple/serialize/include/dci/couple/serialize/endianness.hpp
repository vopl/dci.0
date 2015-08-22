#pragma once
#include <endian.h>

namespace dci { namespace couple { namespace serialize
{

    enum class Endianness
    {
        unknown,
        little,
        big,
        pdp
    };

#if not defined __BYTE_ORDER or not defined __BYTE_ORDER
#   error __BYTE_ORDER must be defined
#endif

    static const Endianness _currentEndianness =
        __LITTLE_ENDIAN == __BYTE_ORDER ?
            Endianness::little :
            __BIG_ENDIAN == __BYTE_ORDER ?
                Endianness::big :
                __PDP_ENDIAN == __BYTE_ORDER ?
                    Endianness::pdp :
                    Endianness::unknown;
    static_assert(Endianness::unknown != _currentEndianness, "");

}}}
