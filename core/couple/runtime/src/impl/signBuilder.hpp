#pragma once

#include "sign.hpp"

#include <string>
#include <cstdint>


namespace dci { namespace couple { namespace runtime { namespace impl
{
    class SignBuilder
    {
    public:
        SignBuilder();
        ~SignBuilder();

        void add(const Sign &v);
        void add(const std::string &v);
        void add(const std::uint32_t &v);
        void add(const char *csz);
        void add(const char *pb, std::size_t size);

        Sign finish();

    private:
        void transform(const std::uint32_t *data);

        std::uint32_t   _state[8];
        std::uint64_t   _bitcount;
        std::uint8_t    _buffer[64];

    };

}}}}
