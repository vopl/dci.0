#pragma once

#include <iostream>
#include <iomanip>
#include <system_error>
#include <boost/system/error_code.hpp>

namespace dci { namespace logger
{
    struct LogStream
    {
        LogStream &operator<<(const std::error_code &ec)
        {
            std::cout << ec.message() << " (" << ec.category().name() << '.' << ec.value() << ')';
            return *this;
        }

        LogStream &operator<<(const boost::system::error_code &ec)
        {
            std::cout << ec.message() << " (" << ec.category().name() << '.' << ec.value() << ')';
            return *this;
        }

        LogStream &operator<<( std::ostream&(*v)(std::ostream&) )
        {
            std::cout<<v;
            return *this;
        }

        template <class T> LogStream &operator<<(const T& v)
        {
            std::cout<<v;
            return *this;
        }
    };

}}

#   define LOGF(msg) {::dci::logger::LogStream ls; ls<<msg<<::std::endl;}
#   define LOGE(msg) {::dci::logger::LogStream ls; ls<<msg<<::std::endl;}
#   define LOGW(msg) {::dci::logger::LogStream ls; ls<<msg<<::std::endl;}
#   define LOGI(msg) {::dci::logger::LogStream ls; ls<<msg<<::std::endl;}
#   define LOGD(msg) {::dci::logger::LogStream ls; ls<<msg<<::std::endl;}
#   define LOGT(msg) {::dci::logger::LogStream ls; ls<<msg<<::std::endl;}

