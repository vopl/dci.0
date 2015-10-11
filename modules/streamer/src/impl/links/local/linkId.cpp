#include "linkId.hpp"

namespace impl { namespace links { namespace local
{

    LinkId::LinkId()
    {
    }

    LinkId::LinkId(std::size_t raw)
        : _raw(raw)
    {
    }

    LinkId LinkId::operator+(LinkId id) const
    {
        return LinkId(_raw + id._raw);
    }

    LinkId LinkId::operator-(LinkId id) const
    {
        return LinkId(_raw - id._raw);
    }

    bool LinkId::operator<(LinkId id) const
    {
        return _raw < id._raw;
    }

    bool LinkId::operator<=(LinkId id) const
    {
        return _raw <= id._raw;
    }

    bool LinkId::operator>(LinkId id) const
    {
        return _raw > id._raw;
    }

    bool LinkId::operator>=(LinkId id) const
    {
        return _raw >= id._raw;
    }

    bool LinkId::operator==(LinkId id) const
    {
        return _raw == id._raw;
    }

    bool LinkId::operator!=(LinkId id) const
    {
        return _raw != id._raw;
    }

    LinkId LinkId::operator+(std::size_t raw) const
    {
        return LinkId(_raw + raw);
    }

    LinkId LinkId::operator-(std::size_t raw) const
    {
        return LinkId(_raw - raw);
    }

    bool LinkId::operator<(std::size_t raw) const
    {
        return _raw < raw;
    }

    bool LinkId::operator<=(std::size_t raw) const
    {
        return _raw <= raw;
    }

    bool LinkId::operator>(std::size_t raw) const
    {
        return _raw > raw;
    }

    bool LinkId::operator>=(std::size_t raw) const
    {
        return _raw >= raw;
    }

    bool LinkId::operator==(std::size_t raw) const
    {
        return _raw == raw;
    }

    bool LinkId::operator!=(std::size_t raw) const
    {
        return _raw != raw;
    }

    LinkId::operator std::size_t() const
    {
        return _raw;
    }



}}}
