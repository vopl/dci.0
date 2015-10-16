#include "signBuilder.hpp"
#include <cstring>
#include <cassert>

#include <iostream>

#if !defined(BYTE_ORDER) || (BYTE_ORDER != LITTLE_ENDIAN && BYTE_ORDER != BIG_ENDIAN)
#error BYTE_ORDER must be defined with LITTLE_ENDIAN or BIG_ENDIAN
#endif






namespace dci { namespace couple { namespace runtime { namespace impl
{
    namespace
    {
        static const std::size_t BLOCK_LENGTH           = 64;
        static const std::size_t SHORT_BLOCK_LENGTH     = (BLOCK_LENGTH - 8);
        static const std::size_t DIGEST_LENGTH          = 32;

        inline std::uint32_t R(std::uint32_t  b, std::uint32_t x)
        {
            return (x >> b);
        }

        inline std::uint32_t S32(std::uint32_t b, std::uint32_t x)
        {
            return ((x >> b) | (x << (32 - b)));
        }

        inline std::uint32_t Ch(std::uint32_t  x, std::uint32_t y, std::uint32_t z)
        {
            return ((x & y) ^ ((~x) & z));
        }
        inline std::uint32_t Maj(std::uint32_t x, std::uint32_t y, std::uint32_t z)
        {
            return ((x & y) ^ (x & z) ^ (y & z));
        }


        inline std::uint32_t Sigma0_256(std::uint32_t x)
        {
            return (S32(2,  x) ^ S32(13, x) ^ S32(22, x));
        }

        inline std::uint32_t Sigma1_256(std::uint32_t x)
        {
            return (S32(6,  x) ^ S32(11, x) ^ S32(25, x));
        }

        inline std::uint32_t sigma0_256(std::uint32_t x)
        {
            return (S32(7,  x) ^ S32(18, x) ^ R(3 ,   x));
        }

        inline std::uint32_t sigma1_256(std::uint32_t x)
        {
            return (S32(17, x) ^ S32(19, x) ^ R(10,   x));
        }

        std::uint16_t REVERSE16(std::uint16_t w)
        {
            std::uint16_t tmp = (w);
            return std::uint16_t(((tmp & std::uint16_t(0xff00UL)) >> 8) | ((tmp & std::uint16_t(0x00ffUL)) << 8));
        }

        std::int16_t REVERSE16(std::int16_t w)
        {
            return std::int16_t(REVERSE16(std::uint16_t(w)));
        }

        std::uint32_t REVERSE32(std::uint32_t w)
        {
            std::uint32_t tmp = (w);
            tmp = (tmp >> 16) | (tmp << 16);
            return std::uint32_t(((tmp & std::uint32_t(0xff00ff00UL)) >> 8) | ((tmp & std::uint32_t(0x00ff00ffUL)) << 8));
        }

        std::int32_t REVERSE32(std::int32_t w)
        {
            return std::int32_t(REVERSE32(std::uint32_t(w)));
        }

        std::uint64_t REVERSE64(std::uint64_t w)
        {
            std::uint64_t tmp = (w);
            tmp = (tmp >> 32) | (tmp << 32);
            tmp = ((tmp & std::uint64_t(0xff00ff00ff00ff00ULL)) >> 8) | ((tmp & std::uint64_t(0x00ff00ff00ff00ffULL)) << 8);

            return std::uint64_t(((tmp & 0xffff0000ffff0000ULL) >> 16) | ((tmp & 0x0000ffff0000ffffULL) << 16));
        }

        std::int64_t REVERSE64(std::int64_t w)
        {
            return std::int64_t(REVERSE64(std::uint64_t(w)));
        }

        const static std::uint32_t K256[64] =
        {
            0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
            0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
            0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
            0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
            0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
            0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
            0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
            0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
            0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
            0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
            0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
            0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
            0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
            0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
            0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
            0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
        };
    }

    SignBuilder::SignBuilder()
        : _state
            {
                0x6a09e667UL,
                0xbb67ae85UL,
                0x3c6ef372UL,
                0xa54ff53aUL,
                0x510e527fUL,
                0x9b05688cUL,
                0x1f83d9abUL,
                0x5be0cd19UL
            }
        , _bitcount{}
        , _buffer{}
    {
    }

    SignBuilder::~SignBuilder()
    {
    }

    void SignBuilder::add(const Sign &v)
    {
        add(v.data(), v._size);
    }

    void SignBuilder::add(const std::string &v)
    {
        add(v.data(), v.size());
    }

    void SignBuilder::add(const char *csz)
    {
        add(csz, strlen(csz));
    }

    void SignBuilder::add(const char *data, std::size_t len)
    {
        unsigned int freespace, usedspace;

        if(len == 0)
        {
            return;
        }

        usedspace = (_bitcount >> 3) % BLOCK_LENGTH;

        if(usedspace > 0)
        {
            freespace = BLOCK_LENGTH - usedspace;

            if(len >= freespace)
            {
                memcpy(&_buffer[usedspace], data, freespace);
                _bitcount += freespace << 3;
                len -= freespace;
                data += freespace;
                transform(reinterpret_cast<std::uint32_t*>(_buffer));
            }
            else
            {
                memcpy(&_buffer[usedspace], data, len);
                _bitcount += len << 3;
                return;
            }
        }
        while (len >= BLOCK_LENGTH)
        {
            transform(reinterpret_cast<const std::uint32_t*>(data));
            _bitcount += BLOCK_LENGTH << 3;
            len -= BLOCK_LENGTH;
            data += BLOCK_LENGTH;
        }
        if(len > 0)
        {
            memcpy(_buffer, data, len);
            _bitcount += len << 3;
        }
    }

    void SignBuilder::add(const std::uint8_t &v)
    {
        auto buf = v;
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::uint16_t &v)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        auto buf = REVERSE16(v);
#else
        auto buf = v;
#endif
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::uint32_t &v)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        auto buf = REVERSE32(v);
#else
        auto buf = v;
#endif
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::uint64_t &v)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        auto buf = REVERSE64(v);
#else
        auto buf = v;
#endif
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::int8_t &v)
    {
        auto buf = v;
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::int16_t &v)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        auto buf = REVERSE16(v);
#else
        auto buf = v;
#endif
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::int32_t &v)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        auto buf = REVERSE32(v);
#else
        auto buf = v;
#endif
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    void SignBuilder::add(const std::int64_t &v)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        auto buf = REVERSE64(v);
#else
        auto buf = v;
#endif
        add(reinterpret_cast<char *>(&buf), sizeof(buf));
    }

    Sign SignBuilder::finish()
    {
        unsigned int usedspace = (_bitcount >> 3) % BLOCK_LENGTH;
#if BYTE_ORDER == LITTLE_ENDIAN
        _bitcount = REVERSE64(_bitcount);
#endif
        if(usedspace > 0)
        {
            _buffer[usedspace++] = 0x80;

            if(usedspace <= SHORT_BLOCK_LENGTH)
            {
                memset(&_buffer[usedspace], 0, SHORT_BLOCK_LENGTH - usedspace);
            }
            else
            {
                if(usedspace < BLOCK_LENGTH)
                {
                    memset(&_buffer[usedspace], 0, BLOCK_LENGTH - usedspace);
                }
                transform(reinterpret_cast<const std::uint32_t*>(_buffer));

                memset(_buffer, 0, SHORT_BLOCK_LENGTH);
            }
        }
        else
        {
            memset(_buffer, 0, SHORT_BLOCK_LENGTH);

            *_buffer = 0x80;
        }

        void *bcPtr = &_buffer[SHORT_BLOCK_LENGTH];
        *static_cast<std::uint64_t*>(bcPtr) = _bitcount;

        transform(reinterpret_cast<const std::uint32_t*>(_buffer));

        Sign digest;
        static_assert(digest._size <= DIGEST_LENGTH, "Sign is too big as a digest here");

#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::uint32_t *d = reinterpret_cast<std::uint32_t*>(digest.data());
            for(std::size_t j = 0; j < digest._size/4; j++)
            {
                _state[j] = REVERSE32(_state[j]);
                *d++ = _state[j];
            }
        }
#else
        memcpy(digest.data(), state, digest._size);
#endif

        return digest;
    }

    void SignBuilder::transform(const std::uint32_t* data)
    {
        std::uint32_t a, b, c, d, e, f, g, h, s0, s1;
        std::uint32_t T1, T2, *W256;

        W256 = reinterpret_cast<std::uint32_t*>(_buffer);

        a = _state[0];
        b = _state[1];
        c = _state[2];
        d = _state[3];
        e = _state[4];
        f = _state[5];
        g = _state[6];
        h = _state[7];

        int j = 0;
        do
        {
#if BYTE_ORDER == LITTLE_ENDIAN
            W256[j] = REVERSE32(*data++);
#else
            W256[j] = *data++;
#endif
            T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + W256[j];

            T2 = Sigma0_256(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;

            j++;
        }
        while (j < 16);

        do
        {
            s0 = W256[(j+1)&0x0f];
            s0 = sigma0_256(s0);
            s1 = W256[(j+14)&0x0f];
            s1 = sigma1_256(s1);

            T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0);
            T2 = Sigma0_256(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;

            j++;
        }
        while (j < 64);

        _state[0] += a;
        _state[1] += b;
        _state[2] += c;
        _state[3] += d;
        _state[4] += e;
        _state[5] += f;
        _state[6] += g;
        _state[7] += h;
    }

//    namespace
//    {
//        int f()
//        {
//            runtime::SignBuilder s;
//            s.add("8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c928d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c9", 127);
//            assert("72dc2022ddbdad86315cf26432cce90ab0fd29e9c20d3c2515963b86cc7d9654" == s.finish().string());
//            exit(0);
//        }
//        static int k = f();
//    }

}}}}
