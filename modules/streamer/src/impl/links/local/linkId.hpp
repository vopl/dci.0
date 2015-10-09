#pragma once
#include <cstdint>

namespace impl { namespace links { namespace local
{
    class LinkId
    {
    public:
        LinkId(std::size_t raw);

        LinkId operator+(LinkId) const;
        LinkId operator-(LinkId) const;
        bool operator<(LinkId) const;
        bool operator<=(LinkId) const;
        bool operator>(LinkId) const;
        bool operator>=(LinkId) const;
        bool operator==(LinkId) const;
        bool operator!=(LinkId) const;

        LinkId operator+(std::size_t) const;
        LinkId operator-(std::size_t) const;
        bool operator<(std::size_t) const;
        bool operator<=(std::size_t) const;
        bool operator>(std::size_t) const;
        bool operator>=(std::size_t) const;
        bool operator==(std::size_t) const;
        bool operator!=(std::size_t) const;

        operator std::size_t() const;

    private:
        std::size_t _raw;
    };

}}}
