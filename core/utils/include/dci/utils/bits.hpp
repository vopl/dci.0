#pragma once
#include <cstdint>
#include <type_traits>

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
            return __builtin_ctz(std::make_unsigned_t<Integral>(x));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(int) && sizeof(Integral) <= sizeof(long)), std::size_t> ctz(Integral x)
        {
            return __builtin_ctzl(std::make_unsigned_t<Integral>(x));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(long) && sizeof(Integral) <= sizeof(long long)), std::size_t> ctz(Integral x)
        {
            return __builtin_ctzll(std::make_unsigned_t<Integral>(x));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) <= sizeof(int)), std::size_t> clz(Integral x)
        {
            return __builtin_clz(std::make_unsigned_t<Integral>(x)) - sizeof(int)*8 + sizeof(Integral)*8;
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(int) && sizeof(Integral) <= sizeof(long)), std::size_t> clz(Integral x)
        {
            return __builtin_clzl(std::make_unsigned_t<Integral>(x));
        }

        template <class Integral> std::enable_if_t<(sizeof(Integral) > sizeof(long) && sizeof(Integral) <= sizeof(long long)), std::size_t> clz(Integral x)
        {
            return __builtin_clzll(std::make_unsigned_t<Integral>(x));
        }
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> least0Count(Integral x)
    {
        if(0 == x)
        {
            return sizeof(x)*8;
        }

        return details::ctz(x);
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> most0Count(Integral x)
    {
        if(0 == x)
        {
            return sizeof(x)*8;
        }

        return sizeof(x)*8 - details::clz(x) - 1;
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> least1Count(Integral x)
    {
        if(Integral(~Integral(0)) == x)
        {
            return sizeof(x)*8;
        }

        return details::ctz(Integral(~x));
    }

    template <class Integral>
    std::enable_if_t<std::is_integral<Integral>::value, std::size_t> most1Count(Integral x)
    {
        if(Integral(~Integral(0)) == x)
        {
            return sizeof(x)*8;
        }

        return sizeof(x)*8 - details::clz(Integral(~x)) - 1;
    }











}}}
