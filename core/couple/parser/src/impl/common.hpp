#pragma once

#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/home/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <memory>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace dci { namespace couple { namespace parser { namespace impl
{
    struct derefEngine
    {
        template<class T> struct result
        {
            typedef T & type;
        };

        template<class T>
        struct result<std::shared_ptr<T>>
        {
            typedef T & type;
        };

        template <class T>
        T &operator()(T &arg1) const
        {
            return arg1;
        }

        template <class T>
        T &operator()(std::shared_ptr<T> arg1) const
        {
            return *arg1;
        }
    };

    extern boost::phoenix::function<derefEngine> deref;

    struct appendEngine
    {
        template<class T1, class T2> struct result
        {
            typedef void type;
        };

        template <class T>
        void operator()(T &arg1, const T &arg2) const
        {
            arg1.insert(arg1.end(), arg2.begin(), arg2.end());
        }
    };

    extern boost::phoenix::function<appendEngine> append;

}}}}
