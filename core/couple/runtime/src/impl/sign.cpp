#include "sign.hpp"
#include <cassert>

namespace dci { namespace couple { namespace runtime { namespace impl
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

    std::string Sign::toHex(std::size_t chars) const
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

    bool Sign::fromHex(const std::string &txt)
    {
        if(txt.size() < 64)
        {
            return false;
        }

        for(std::size_t i(0); i<32; ++i)
        {
            unsigned int h = static_cast<unsigned char>(txt[i*2+0]);
            unsigned int l = static_cast<unsigned char>(txt[i*2+1]);

            if(h >= 'a' && h <='f') h -= 'a'-10;
            else if(h >= 'A' && h <='F') h -= 'A'-10;
            else if(h >= '0' && h <='9') h -= '0';
            else return false;

            if(l >= 'a' && l <='f') l -= 'a'-10;
            else if(l >= 'A' && l <='F') l -= 'A'-10;
            else if(l >= '0' && l <='9') l -= '0';
            else return false;

            _data._by8[i] = h<<4 | l;
        }

        return true;
    }

}}}}
