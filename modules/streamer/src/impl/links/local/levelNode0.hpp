#pragma once
#include "levelNodeBase.hpp"
#include <dci/utils/bits.hpp>

namespace impl { namespace links { namespace local
{
    template <class Cfg>
    class LevelNode<Cfg, 0>
        : public LevelNodeBase<Cfg>
        , public dci::mm::NewDelete<LevelNode<Cfg, 0>>
    {
    public:
        using Parent = LevelNode<Cfg, 1>;

        using Container = typename Cfg::Container;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;
        static const std::size_t _volume = _width * 1;

        using Mask = std::size_t;
        static const Mask _fullUseMask = (bitsof(Mask)==_width ? (~std::size_t(0)) : ((1ull<<_width) - 1));

        using LinkPtr = std::unique_ptr<Link>;

    public:
        LevelNode();
        ~LevelNode();

        LinkId add(Container *container, LinkPtr &&link);
        LinkId add(LinkPtr &&link);
        Link *get(const LinkId &id) const;
        LinkPtr del(Container *container, const LinkId &id);

        bool isFull() const;

    private:
        Mask _useMask = 0;
        static_assert(bitsof(_useMask) >= _width, "width must not be greater then _useMask");
        LinkPtr _links[_width] = {};
    };










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNode<Cfg, 0>::LevelNode()
        : LevelNodeBase<Cfg>()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LevelNode<Cfg, 0>::~LevelNode()
    {
        assert(!_useMask);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LinkId LevelNode<Cfg, 0>::add(Container *container, LinkPtr &&link)
    {
        LinkId id = dci::utils::bits::least1Count(_useMask);

        assert(id <= _width);
        if(id >= _width)
        {
            return container->levelUpAdd(std::move(link));
        }

        assert(link && "null link added?");
        assert(!_links[id]);
        _links[id] = std::move(link);
        _useMask |= 1ull<<id;

        return id;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    LinkId LevelNode<Cfg, 0>::add(LinkPtr &&link)
    {
        LinkId id = dci::utils::bits::least1Count(_useMask);

        assert(id <= _width);

        assert(link && "null link added?");
        assert(!_links[id]);
        _links[id] = std::move(link);
        _useMask |= 1ull<<id;

        return id;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename LevelNode<Cfg, 0>::Link *LevelNode<Cfg, 0>::get(const LinkId &id) const
    {
        assert(id < _width);
        assert(_links[id]);
        assert((_useMask>>id) & 1);

        return _links[id].get();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename LevelNode<Cfg, 0>::LinkPtr LevelNode<Cfg, 0>::del(Container *container, const LinkId &id)
    {
        assert(id < _width);
        assert(_links[id]);
        assert((_useMask>>id) & 1);

        (void)container;
        assert(!"remove empty nodes");

        _useMask &= ~(1u<<id);
        return std::move(_links[id]);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool LevelNode<Cfg, 0>::isFull() const
    {
        return _fullUseMask == _useMask;
    }

}}}
