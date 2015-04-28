#include <signBuilder.hpp>
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

    void SignBuilder::add(const std::uint32_t &v)
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

    Sign SignBuilder::finish()
    {
        return himpl::impl2Face<Sign>(impl().finish());
    }


}}}
