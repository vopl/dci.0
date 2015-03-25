#pragma once

#include "task.hpp"
#include "../future.hpp"
#include <dci/mm/functions.hpp>

#include <functional>
#include <type_traits>
#include <cstddef>


namespace dci { namespace async { namespace details
{
    template<int> struct placeholder_template{};
}}}

namespace std
{
    template<int N>
    struct is_placeholder<dci::async::details::placeholder_template<N>>
        : integral_constant<int, N+1>
    {};
}

namespace dci { namespace async { namespace details
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    class TaskInstance
        : public Task
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
        struct MkPromices
        {
            using result = std::tuple<>;
        };

        template <class... ArgsTail>
        struct MkPromices<std::tuple<Args..., ArgsTail...>>
        {
            using result = std::tuple<std::decay_t<ArgsTail>...>;
        };

        template <class T> struct MkFutures;
        template <class... Types>
        struct MkFutures<std::tuple<Promise<Types>...>>
        {
            using result = std::tuple<Future<Types>...>;
        };

        template <class Promices>
        struct MkCall
        {
            template <class T> struct S;

            template <int... phi>
            struct S<std::integer_sequence<int, phi...>>
            {
                using result = decltype(std::bind(std::declval<F>(), std::declval<Args>()..., placeholder_template<phi>{}...));
            };

            using result = typename S<std::make_integer_sequence<int, std::tuple_size<Promices>::value>>::result;
        };

    public:
        static TaskInstance *alloc(F &&f, Args &&...args);

        using AllArgs = decltype(mkArgs(std::declval<F>()));
        using Promices = typename MkPromices<AllArgs>::result;
        using Futures = typename MkFutures<Promices>::result;

        template <class T>
        struct SpawnerResultTypeEvaluator
        {
            using result = void;
            static result getSpawnerResult(Promices &)
            {

            }
        };

        template <class T>
        struct SpawnerResultTypeEvaluator<std::tuple<T>>
        {
            using result = T;
            static result getSpawnerResult(Promices &promices)
            {
                return std::get<0>(promices).future();
            }
        };

        template <class T1, class T2, class... TN>
        struct SpawnerResultTypeEvaluator<std::tuple<T1, T2, TN...>>
        {
            using result = std::tuple<T1, T2, TN...>;

            template <int... phi>
            static result getSpawnerResult(Promices &promices, std::integer_sequence<int, phi...>)
            {
                return std::make_tuple(std::get<phi>(promices).future()...);
            }

            static result getSpawnerResult(Promices &promices)
            {
                return getSpawnerResult(promices, std::make_integer_sequence<int, std::tuple_size<Promices>::value>());
            }
        };

        using SpawnerResultType = typename SpawnerResultTypeEvaluator<Futures>::result;

        SpawnerResultType getSpawnerResult()
        {
            return SpawnerResultTypeEvaluator<Futures>::getSpawnerResult(_promices);
        }

    private:
        static void callExecutor(Task *task);
        static void freeExecutor(Task *task);

    private:
        TaskInstance(F &&f, Args &&...args);
        ~TaskInstance();

    public:
        using Call = typename MkCall<Promices>::result;
        Call _call;

        template<int... phi>
        void doCall(const std::integer_sequence<int, phi...> &)
        {
            _call(std::move(std::get<phi>(_promices))...);
        }

    public:
        Promices _promices;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...> *TaskInstance<F, Args...>::alloc(F &&f, Args &&...args)
    {
        return new(mm::alloc<sizeof(TaskInstance)>()) TaskInstance(std::forward<F>(f), std::forward<Args>(args)...);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::callExecutor(Task *task)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);
        return self->doCall(std::make_integer_sequence<int, std::tuple_size<Promices>::value>());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    void TaskInstance<F, Args...>::freeExecutor(Task *task)
    {
        TaskInstance *self = static_cast<TaskInstance*>(task);

        self->~TaskInstance();

        mm::free<sizeof(TaskInstance)>(self);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::TaskInstance(F &&f, Args &&...args)
        : Task(&TaskInstance::callExecutor, &TaskInstance::freeExecutor)
        , _call(std::forward<F>(f), std::forward<Args>(args)...)
    {
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class F, class... Args>
    TaskInstance<F, Args...>::~TaskInstance()
    {
    }

}}}
