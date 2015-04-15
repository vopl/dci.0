#include "sign.hpp"
#include <cassert>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Sign::Data::Data()
        : _by8{0,0,0,0}
    {
    }

    Sign::Sign()
    {

    }

    Sign::~Sign()
    {

    }

    char *Sign::data()
    {
        return (char *)_data._by8;
    }

    const char *Sign::data() const
    {
        return (const char *)_data._by8;
    }

    std::string Sign::string(std::size_t chars) const
    {
        if(chars > 64)
        {
            chars = 64;
        }

        std::string res;
        res.resize(chars);

        for(std::size_t c(0); c<chars; ++c)
        {
            std::uint8_t u = _data._by8[c/2];
            if(c & 1)
            {
                u &= 0x0f;
            }
            else
            {
                u >>= 4;
            }

            if(u<10)
            {
                res[c] = '0'+u;
            }
            else
            {
                res[c] = 'a'+u-10;
            }
        }

        return res;
    }


}}}}
