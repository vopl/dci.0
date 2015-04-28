#include <sign.hpp>
#include "impl/sign.hpp"

namespace dci { namespace couple { namespace runtime
{
    Sign::Sign()
        : ThisFaceLayout()
    {
    }

    Sign::Sign(const Sign &from)
        : ThisFaceLayout(from.impl())
    {
    }

    Sign::~Sign()
    {
    }

    Sign &Sign::operator=(const Sign &from)
    {
        impl() = from.impl();
        return *this;
    }

    std::string Sign::toHex(std::size_t chars) const
    {
        return impl().toHex(chars);
    }

    bool Sign::fromHex(const std::string &txt)
    {
        return impl().fromHex(txt);
    }
}}}
