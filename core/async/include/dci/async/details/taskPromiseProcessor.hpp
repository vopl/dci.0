#pragma once

#include "../future.hpp"

#include <functional>
#include <type_traits>
#include <cstddef>

namespace dci { namespace async { namespace details
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<int> struct BindPlaceholder {};

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    class TaskPromiseProcessor
    {
    private:
        template <class F1, class R1, class... Args1>
        static std::tuple<Args1...> mkArgs(R1(F1::*)(Args1...) const);

        template <class F1, class R1, class... Args1>
        static std::tuple<Args1...> mkArgs(R1(F1::*)(Args1...) volatile);

        template <class F1, class R1, class... Args1>
        static std::tuple<Args1...> mkArgs(R1(F1::*)(Args1...) const volatile);

        template <class F1, class R1, class... Args1>
        static std::tuple<Args1...> mkArgs(R1(F1::*)(Args1...));

        template <class R1, class... Args1>
        static std::tuple<Args1...> mkArgs(R1(*)(Args1...));

        template <class F1>
        static std::enable_if_t<(sizeof(&F1::operator())>0), decltype(mkArgs(&F1::operator()))> mkArgs(F1 f);

        static std::tuple<> mkArgs(...);


        template <class T>
        struct MkPromises
        {
            using result = std::tuple<>;
        };

        template <class... ArgsTail>
        struct MkPromises<std::tuple<Args..., ArgsTail...>>
        {
            using result = std::tuple<std::decay_t<ArgsTail>...>;
        };

        template <class T> struct MkFutures;
        template <class... Promises>
        struct MkFutures<std::tuple<Promises...>>
        {
            using result = std::tuple<typename Promises::Future...>;
        };

        template <class Promises>
        struct MkCall
        {
            template <class T> struct Expander;

            template <int... phi>
            struct Expander<std::integer_sequence<int, phi...>>
            {
                using result = decltype(std::bind(std::declval<F>(), std::declval<Args>()..., BindPlaceholder<phi>{}...));
            };

            using result = typename Expander<std::make_integer_sequence<int, std::tuple_size<Promises>::value>>::result;
        };

        using AllArgs = decltype(mkArgs(std::declval<F>()));

    public:
        using Promises = typename MkPromises<AllArgs>::result;

    private:
        using Futures = typename MkFutures<Promises>::result;

        template <class T>
        struct SpawnerResultTypeEvaluator
        {
            using result = void;
            static result getSpawnerResult(Promises &)
            {

            }
        };

        template <class T>
        struct SpawnerResultTypeEvaluator<std::tuple<T>>
        {
            using result = T;
            static result getSpawnerResult(Promises &promises)
            {
                return std::get<0>(promises).future();
            }
        };

        template <class T1, class T2, class... TN>
        struct SpawnerResultTypeEvaluator<std::tuple<T1, T2, TN...>>
        {
            using result = std::tuple<T1, T2, TN...>;

            template <int... phi>
            static result getSpawnerResult(Promises &promises, std::integer_sequence<int, phi...>)
            {
                return std::make_tuple(std::get<phi>(promises).future()...);
            }

            static result getSpawnerResult(Promises &promises)
            {
                return getSpawnerResult(promises, std::make_integer_sequence<int, std::tuple_size<Promises>::value>());
            }
        };

    public:
        using SpawnerResultType = typename SpawnerResultTypeEvaluator<Futures>::result;

        static SpawnerResultType getSpawnerResult(Promises &promises)
        {
            return SpawnerResultTypeEvaluator<Futures>::getSpawnerResult(promises);
        }

        using Call = typename MkCall<Promises>::result;

    private:
        template<int... phi>
        static void callArgsExpander(Call &call, Promises &promises, const std::integer_sequence<int, phi...> &)
        {
            call(std::move(std::get<phi>(promises))...);
        }

    public:
        static void doCall(Call &call, Promises &promises)
        {
            callArgsExpander(call, promises, std::make_integer_sequence<int, std::tuple_size<Promises>::value>());
        }

    };

}}}

namespace std
{
    template<int N>
    struct is_placeholder<dci::async::details::BindPlaceholder<N>>
        : integral_constant<int, N+1>
    {};
}
