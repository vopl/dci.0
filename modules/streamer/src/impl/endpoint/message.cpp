#include "message.hpp"

namespace impl { namespace endpoint
{

    namespace
    {
        enum class SmallUnsignedMode
        {
            _1234,
            _0124,
        };

        template <SmallUnsignedMode mode>
        std::size_t smallUnsignedLength(std::uint8_t flags)
        {
            switch(mode)
            {
            case SmallUnsignedMode::_1234:
                switch(flags)
                {
                case 0: return 1;
                case 1: return 2;
                case 2: return 3;
                case 3: return 4;
                }
                break;

            case SmallUnsignedMode::_0124:
                switch(flags)
                {
                case 0: return 0;
                case 1: return 1;
                case 2: return 2;
                case 3: return 4;
                }
                break;
            }

            assert(!"never here");
            return 0;
        }

        template <SmallUnsignedMode mode>
        std::uint8_t writeSmallUnsigned(std::uint32_t su, std::uint8_t *data, std::size_t &dataLen)
        {
            switch(mode)
            {
            case SmallUnsignedMode::_1234:
                {
                    if(su < 1<<8)
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        return 0;
                    }
                    else if(su < 1<<16)
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        data[dataLen++] = (std::uint8_t)(su >> 8);
                        return 1;
                    }
                    else if(su < 1<<24)
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        data[dataLen++] = (std::uint8_t)(su >> 8);
                        data[dataLen++] = (std::uint8_t)(su >> 16);
                        return 2;
                    }
                    else
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        data[dataLen++] = (std::uint8_t)(su >> 8);
                        data[dataLen++] = (std::uint8_t)(su >> 16);
                        data[dataLen++] = (std::uint8_t)(su >> 24);
                        return 3;
                    }
                }
                break;
            case SmallUnsignedMode::_0124:
                {
                    if(!su)
                    {
                        return 0;
                    }
                    if(su < 1<<8)
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        return 1;
                    }
                    else if(su < 1<<16)
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        data[dataLen++] = (std::uint8_t)(su >> 8);
                        return 2;
                    }
                    else
                    {
                        data[dataLen++] = (std::uint8_t)(su >> 0);
                        data[dataLen++] = (std::uint8_t)(su >> 8);
                        data[dataLen++] = (std::uint8_t)(su >> 16);
                        data[dataLen++] = (std::uint8_t)(su >> 24);
                        return 3;
                    }
                }
                break;
            }
        }

        template <SmallUnsignedMode mode>
        std::uint32_t readSmallUnsigned(std::uint8_t flags, std::uint8_t *data, std::size_t &dataLen)
        {
            switch(mode)
            {
            case SmallUnsignedMode::_1234:
                switch(flags)
                {
                case 0:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        return sm;
                    }
                case 1:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        sm |= data[dataLen++] << 8;
                        return sm;
                    }
                case 2:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        sm |= data[dataLen++] << 8;
                        sm |= data[dataLen++] << 16;
                        return sm;
                    }
                case 3:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        sm |= data[dataLen++] << 8;
                        sm |= data[dataLen++] << 16;
                        sm |= data[dataLen++] << 24;
                        return sm;
                    }
                }
                break;

            case SmallUnsignedMode::_0124:
                switch(flags)
                {
                case 0:
                    {
                        std::uint32_t sm = 0;
                        return sm;
                    }
                case 1:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        return sm;
                    }
                case 2:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        sm |= data[dataLen++] << 8;
                        return sm;
                    }
                case 3:
                    {
                        std::uint32_t sm = 0;
                        sm |= data[dataLen++] << 0;
                        sm |= data[dataLen++] << 8;
                        sm |= data[dataLen++] << 16;
                        sm |= data[dataLen++] << 24;
                        return sm;
                    }
                }
                break;
            }

            assert(!"never here");
            return 0;
        }

    }

    bool deserializeHeader(Bytes &data, Message &msg, std::uint32_t &bodyLength)
    {
        if(data.size()<1)
        {
            return false;
        }

        std::size_t headerLen = 1;
        std::uint8_t flags = data[0];

        headerLen += smallUnsignedLength<SmallUnsignedMode::_1234>((flags>>0) & 0x3);
        headerLen += smallUnsignedLength<SmallUnsignedMode::_0124>((flags>>2) & 0x3);
        headerLen += smallUnsignedLength<SmallUnsignedMode::_0124>((flags>>4) & 0x3);

        if(headerLen < data.size())
        {
            return false;
        }

        std::uint8_t header[16];
        data.fillAndDropFirst(header, headerLen);

        std::size_t headerLen2 = 1;
        msg._type   = readSmallUnsigned<SmallUnsignedMode::_1234>((flags>>0) & 0x3, header, headerLen2);
        msg._linkId = readSmallUnsigned<SmallUnsignedMode::_0124>((flags>>2) & 0x3, header, headerLen2);
        bodyLength  = readSmallUnsigned<SmallUnsignedMode::_0124>((flags>>4) & 0x3, header, headerLen2);
        assert(headerLen == headerLen2);

        data.dropFirst(headerLen);
        return true;
    }

    Bytes serialize(Message &&msg)
    {
        std::uint8_t header[16];
        header[0] = 0;
        std::size_t headerLen = 1;

        header[0] |= writeSmallUnsigned<SmallUnsignedMode::_1234>(msg._type, header, headerLen);
        header[0] |= writeSmallUnsigned<SmallUnsignedMode::_0124>(msg._linkId, header, headerLen)<<2;
        header[0] |= writeSmallUnsigned<SmallUnsignedMode::_0124>(msg._body.size(), header, headerLen)<<4;

        Bytes res;
        res.append((const char *)&header, headerLen);
        res.append(std::move(msg._body));
        return res;
    }
}}
