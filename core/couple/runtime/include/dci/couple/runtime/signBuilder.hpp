#pragma once

#include "sign.hpp"

#include <string>
#include <cstdint>


namespace dci { namespace couple { namespace runtime
{
    namespace impl
    {
        class SignBuilder;
    }

    class APIDCI_COUPLE_RUNTIME SignBuilder
        : public himpl::FaceLayout<impl::SignBuilder>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        SignBuilder();
        ~SignBuilder();

        void add(const Sign &v);
        void add(const std::string &v);
        void add(const char *csz);
        void add(const char *pb, std::size_t size);

        void add(const std::uint8_t &v);
        void add(const std::uint16_t &v);
        void add(const std::uint32_t &v);
        void add(const std::uint64_t &v);

        void add(const std::int8_t &v);
        void add(const std::int16_t &v);
        void add(const std::int32_t &v);
        void add(const std::int64_t &v);

        Sign finish();
    };

}}}
