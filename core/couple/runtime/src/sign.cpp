#include <dci/couple/runtime/sign.hpp>
#include "impl/sign.hpp"

namespace dci { namespace couple { namespace runtime
{
    Sign::Sign()
        : ThisFaceLayout()
    {
    }

    Sign::Sign(const std::uint8_t (&data)[16])
        : ThisFaceLayout(data)
    {
    }

    Sign::Sign(const Sign &from)
        : ThisFaceLayout(from.impl())
    {
    }

    Sign::Sign(Sign &&from)
        : ThisFaceLayout(std::move(from.impl()))
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

    Sign &Sign::operator=(Sign &&from)
    {
        impl() = std::move(from.impl());
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

    void Sign::fromRnd()
    {
        impl().fromRnd();
    }

    bool Sign::operator<(const Sign &with) const
    {
        return impl() < with.impl();
    }

    bool Sign::operator>(const Sign &with) const
    {
        return impl() > with.impl();
    }

    bool Sign::operator<=(const Sign &with) const
    {
        return impl() <= with.impl();
    }

    bool Sign::operator>=(const Sign &with) const
    {
        return impl() >= with.impl();
    }

    bool Sign::operator==(const Sign &with) const
    {
        return impl() == with.impl();
    }

    bool Sign::operator!=(const Sign &with) const
    {
        return impl() != with.impl();
    }

}}}
