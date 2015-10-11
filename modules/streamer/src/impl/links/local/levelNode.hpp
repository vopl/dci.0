#pragma once
#include <dci/mm/newDelete.hpp>
#include "levelNodeBase.hpp"

namespace impl { namespace links { namespace local
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    class LevelNode
        : public LevelNodeBase<Cfg>
        , public dci::mm::NewDelete<LevelNode<Cfg, level>>
    {
    public:
        using Parent = LevelNode<Cfg, level+1>;
        using Child = LevelNode<Cfg, level-1>;

        using Container = typename Cfg::Container;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;
        static const std::size_t _volume = _width * Child::_volume;

        using Mask = std::size_t;
        static const Mask _fullFullMask = (bitsof(Mask)==_width ? (~std::size_t(0)) : ((1ull<<_width) - 1));

        using LinkPtr = std::unique_ptr<Link>;
        using ChildPtr = std::unique_ptr<Child>;

    public:
        LevelNode();
        LevelNode(ChildPtr &&child);
        ~LevelNode();

        LinkId add(Container *container, LinkPtr &&link);
        LinkId add(LinkPtr &&link);
        Link *get(const LinkId &id) const;
        LinkPtr del(Container *container, const LinkId &id);

        bool isFull() const;

    private:
        Mask _fullMask = 0;
        static_assert(bitsof(_fullMask) >= _width, "width must not be greater then _fullMask");
        ChildPtr _children[_width] = {};
    };







    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LevelNode<Cfg, level>::LevelNode()
        : LevelNodeBase<Cfg>()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LevelNode<Cfg, level>::LevelNode(ChildPtr &&child)
        : LevelNodeBase<Cfg>()
        , _fullMask(1ull<<0)
    {
        assert(child->isFull());
        _children[0] = std::move(child);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LevelNode<Cfg, level>::~LevelNode()
    {
        assert(!_fullMask);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LinkId LevelNode<Cfg, level>::add(Container *container, LinkPtr &&link)
    {
        LinkId id = dci::utils::bits::least1Count(_fullMask);

        assert(id <= _width);
        if(id >= _width)
        {
            return container->levelUpAdd(std::move(link));
        }

        assert(link && "null link added?");

        LinkId childId;

        if(!_children[id])
        {
            _children[id].reset(new Child());
            childId = _children[id]->add(std::move(link));
        }
        else
        {
            assert(!_children[id]->isFull());

            childId = _children[id]->add(std::move(link));

            if(_children[id]->isFull())
            {
                assert(childId == Child::_volume-1);
                _fullMask |= 1ull<<id;
            }
        }

        return id * Child::_volume + childId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LinkId LevelNode<Cfg, level>::add(LinkPtr &&link)
    {
        LinkId id = dci::utils::bits::least1Count(_fullMask);

        assert(id <= _width);

        assert(link && "null link added?");

        LinkId childId;

        if(!_children[id])
        {
            _children[id].reset(new Child());
            childId = _children[id]->add(std::move(link));
        }
        else
        {
            assert(!_children[id]->isFull());

            childId = _children[id]->add(std::move(link));

            if(_children[id]->isFull())
            {
                assert(childId == Child::_volume-1);
                _fullMask |= 1ull<<id;
            }
        }

        return id * Child::_volume + childId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename LevelNode<Cfg, level>::Link *LevelNode<Cfg, level>::get(const LinkId &id) const
    {
        (void)id;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename LevelNode<Cfg, level>::LinkPtr LevelNode<Cfg, level>::del(Container *container, const LinkId &id)
    {
        (void)container;
        (void)id;
        assert(0);
        return LinkPtr();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool LevelNode<Cfg, level>::isFull() const
    {
        return _fullFullMask == _fullMask;
    }

}}}
