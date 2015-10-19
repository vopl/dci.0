#pragma once
#include <boost/preprocessor/repetition/repeat.hpp>
#include <tuple>
#include <type_traits>

namespace dci { namespace utils
{
    template <class Sequence>
    struct SwitchExpander
    {

    public:
        template <class F>
        static auto exec(typename Sequence::value_type value, F &&f)
        {
            return Impl<Sequence::size(), F>::exec(value, std::move(f));
        }


    private:

        template<class T> struct Mod;

        template<class Integral, Integral... values> struct Mod<std::integer_sequence<Integral, values...>>
        {
            using tvalues = std::tuple<std::integral_constant<Integral, values>...>;
        };

        using tvalues = typename Mod<Sequence>::tvalues;

        template <std::size_t index>
        using tvalue = std::tuple_element_t<index, tvalues>;

    private:
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        template <std::size_t amount, class F> struct Impl;



#define _98302384578923645975873452345_entry(z, n, text)  case tvalue<n>::value: return f(tvalue<n>());

#define _98302384578923645975873452345(n)                                                           \
        template <class F>                                                                          \
        struct Impl<n, F>                                                                           \
        {                                                                                           \
            static std::result_of_t<F(tvalue<0>)> exec(typename Sequence::value_type value, F &&f)  \
            {                                                                                       \
                switch(value)                                                                       \
                {                                                                                   \
                    BOOST_PP_REPEAT(n, _98302384578923645975873452345_entry, )                      \
                }                                                                                   \
                assert(0&&"never here");                                                            \
                __builtin_unreachable();                                                            \
            }                                                                                       \
        };                                                                                          \
                                                                                                 /**/

        _98302384578923645975873452345(1)
        _98302384578923645975873452345(2)
        _98302384578923645975873452345(3)
        _98302384578923645975873452345(4)
        _98302384578923645975873452345(5)
        _98302384578923645975873452345(6)
        _98302384578923645975873452345(7)
        _98302384578923645975873452345(8)
        _98302384578923645975873452345(9)
        _98302384578923645975873452345(10)
        _98302384578923645975873452345(11)
        _98302384578923645975873452345(12)
        _98302384578923645975873452345(13)
        _98302384578923645975873452345(14)
        _98302384578923645975873452345(15)
        _98302384578923645975873452345(16)
        _98302384578923645975873452345(17)
        _98302384578923645975873452345(18)
        _98302384578923645975873452345(19)
        _98302384578923645975873452345(20)

#undef _98302384578923645975873452345_entry
#undef _98302384578923645975873452345

    };


}}
