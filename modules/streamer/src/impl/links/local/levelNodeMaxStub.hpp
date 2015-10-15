#pragma once
#include "levelNodeBase.hpp"

namespace impl { namespace links { namespace local
{
    template <class Cfg>
    struct LevelNodeMaxStub
        : public LevelNodeBase<Cfg>
    {
        static const std::size_t _level = 100500;

        LevelNodeMaxStub(...)
        {
            assert(!"never here");
        }

        LinkId add(void *, void *)
        {
            assert(!"never here");
            return Cfg::_badLinkId;
        }

        bool add(void *, std::size_t, void *)
        {
            assert(!"never here");
            return false;
        }

    };

}}}
