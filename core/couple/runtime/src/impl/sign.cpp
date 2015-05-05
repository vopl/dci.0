#include "sign.hpp"
#include <cassert>
#include <cstring>
#include <random>

namespace dci { namespace couple { namespace runtime { namespace impl
{
    Sign::Sign()
        : _data{}
    {

    }

    Sign::Sign(const std::uint8_t (&data)[_size])
    {
        memcpy(this->data(), data, _size);
    }

    Sign::Sign(const Sign &from)
    {
        memcpy(data(), from.data(), _size);
    }

    Sign::Sign(Sign &&from)
    {
        memcpy(data(), from.data(), _size);
    }

    Sign::~Sign()
    {

    }

    Sign &Sign::operator=(const Sign &from)
    {
        memcpy(data(), from.data(), _size);
        return *this;
    }

    Sign &Sign::operator=(Sign &&from)
    {
        memcpy(data(), from.data(), _size);
        return *this;
    }

    char *Sign::data()
    {
        return (char *)_data;
    }

    const char *Sign::data() const
    {
        return (const char *)_data;
    }

    std::string Sign::toHex(std::size_t chars) const
    {
        if(chars > _size*2)
        {
            chars = _size*2;
        }

        std::string res;
        res.resize(chars);

        for(std::size_t c(0); c<chars; ++c)
        {
            std::uint8_t u = _data[c/2];
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
        if(txt.size() < _size*2)
        {
            return false;
        }

        for(std::size_t i(0); i<_size; ++i)
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

            _data[i] = h<<4 | l;
        }

        return true;
    }

    void Sign::fromRnd()
    {
        static std::random_device rd;
        std::uniform_int_distribution<std::uint8_t> dist;
        for(std::size_t i(0); i<_size; ++i)
        {
            _data[i] = dist(rd);
        }
    }

    bool Sign::operator<(const Sign &with) const
    {
        return memcmp(data(), with.data(), _size) < 0;
    }

    bool Sign::operator>(const Sign &with) const
    {
        return memcmp(data(), with.data(), _size) > 0;
    }

    bool Sign::operator<=(const Sign &with) const
    {
        return memcmp(data(), with.data(), _size) <= 0;
    }

    bool Sign::operator>=(const Sign &with) const
    {
        return memcmp(data(), with.data(), _size) >= 0;
    }

    bool Sign::operator==(const Sign &with) const
    {
        return memcmp(data(), with.data(), _size) == 0;
    }

    bool Sign::operator!=(const Sign &with) const
    {
        return memcmp(data(), with.data(), _size) != 0;
    }


}}}}
