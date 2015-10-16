#pragma once
#include <cstdint>
#include <type_traits>
#include <climits>


#define bitsof(x) (sizeof(x)*CHAR_BIT)

namespace dci { namespace utils { namespace bits
{
    template <class Integral> std::enable_if_t<std::is_integral<Integral>::value, std::size_t> least0Count(Integral x);
    template <class Integral> std::enable_if_t<std::is_integral<Integral>::value, std::size_t> most0Count(Integral x);
    template <class Integral> std::enable_if_t<std::is_integral<Integral>::value, std::size_t> least1Count(Integral x);
    template <class Integral> std::enable_if_t<std::is_integral<Integral>::value, std::size_t> most1Count(Integral x);



    namespace details
    {
        template <class Integral> std::enable_if_t<(sizeof(Integral) <= sizeof(int)), std::size_t> ctz(Integral x)
        {
            return static_cast<std::size_t>(__builtin_ctz(std::make_unsigned_t<Integral>(x)));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(int) && sizeof(Integral) <= sizeof(long)), std::size_t> ctz(Integral x)
        {
            return static_cast<std::size_t>(__builtin_ctzl(std::make_unsigned_t<Integral>(x)));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(long) && sizeof(Integral) <= sizeof(long long)), std::size_t> ctz(Integral x)
        {
            return static_cast<std::size_t>(__builtin_ctzll(std::make_unsigned_t<Integral>(x)));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) <= sizeof(int)), std::size_t> clz(Integral x)
        {
            return static_cast<std::size_t>(static_cast<std::size_t>(__builtin_clz(std::make_unsigned_t<Integral>(x))) - bitsof(int) + bitsof(Integral));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(int) && sizeof(Integral) <= sizeof(long)), std::size_t> clz(Integral x)
        {
            return static_cast<std::size_t>(__builtin_clzl(std::make_unsigned_t<Integral>(x)));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(long) && sizeof(Integral) <= sizeof(long long)), std::size_t> clz(Integral x)
        {
            return static_cast<std::size_t>(__builtin_clzll(std::make_unsigned_t<Integral>(x)));
        }
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> least0Count(Integral x)
    {
        if(0 == x)
        {
            return bitsof(x);
        }

        return details::ctz(x);
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> most0Count(Integral x)
    {
        if(0 == x)
        {
            return bitsof(x);
        }

        return bitsof(x) - details::clz(x) - 1;
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> least1Count(Integral x)
    {
        if(Integral(~Integral(0)) == x)
        {
            return bitsof(x);
        }

        return details::ctz(Integral(~x));
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> most1Count(Integral x)
    {
        if(Integral(~Integral(0)) == x)
        {
            return bitsof(x);
        }

        return bitsof(x) - details::clz(Integral(~x)) - 1;
    }











}}}
