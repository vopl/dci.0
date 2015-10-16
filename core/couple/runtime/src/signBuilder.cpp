#include <dci/couple/runtime/signBuilder.hpp>
#include "impl/signBuilder.hpp"

namespace dci { namespace couple { namespace runtime
{
    SignBuilder::SignBuilder()
        : ThisFaceLayout()
    {

    }

    SignBuilder::~SignBuilder()
    {
    }

    void SignBuilder::add(const Sign &v)
    {
        impl().add(himpl::face2Impl(v));
    }

    void SignBuilder::add(const std::string &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const char *csz)
    {
        impl().add(csz);
    }

    void SignBuilder::add(const char *pb, std::size_t size)
    {
        impl().add(pb, size);
    }

    void SignBuilder::add(const std::uint8_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::uint16_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::uint32_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::uint64_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::int8_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::int16_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::int32_t &v)
    {
        impl().add(v);
    }

    void SignBuilder::add(const std::int64_t &v)
    {
        impl().add(v);
    }

    Sign SignBuilder::finish()
    {
        return himpl::impl2Face<Sign>(impl().finish());
    }

}}}
