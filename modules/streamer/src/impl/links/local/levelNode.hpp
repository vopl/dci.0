#pragma once
#include <dci/mm/newDelete.hpp>
#include "levelNodeMaxStub.hpp"

namespace impl { namespace links { namespace local
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    class LevelNode
        : public LevelNodeBase<Cfg>
        , public dci::mm::NewDelete<LevelNode<Cfg, level>>
    {
    public:
        using Parent = std::conditional_t<
            (level<Cfg::_levels),
            LevelNode<Cfg, level+1>,
            LevelNodeMaxStub<Cfg>>;

        using Child = LevelNode<Cfg, level-1>;

        using Container = typename Cfg::Container;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;
        static const std::size_t _level = level;
        static const std::size_t _volume = _width * Child::_volume;

        using Mask = std::size_t;
        static const Mask _fullFullMask = (bitsof(Mask)==_width ? (~std::size_t(0)) : ((1ull<<_width) - 1));

    public:
        LevelNode();
        LevelNode(Child *child);
        ~LevelNode();

        LinkId add(Container *container, Link *link);
        LinkId add(Link *link);

        bool add(Container *container, LinkId id, Link *link);
        bool add(LinkId id, Link *link);

        Link *get(const LinkId &id) const;
        Link *del(Container *container, const LinkId &id);
        Link *del(const LinkId &id);

        void probablyDown(Container *container);

        bool isEmptyExceptFirst() const;
        bool isEmpty() const;
        bool isFull() const;

    private:
        Mask _useMask = 0;
        Mask _fullMask = 0;
        static_assert(bitsof(_fullMask) >= _width, "width must not be greater then _fullMask");
        Child *_children[_width] = {};
    };





    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LevelNode<Cfg, level>::LevelNode()
        : LevelNodeBase<Cfg>()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LevelNode<Cfg, level>::LevelNode(Child *child)
        : LevelNodeBase<Cfg>()
        , _useMask(1ull<<0)
        , _fullMask(1ull<<0)
    {
        //assert(child->isFull());
        _children[0] = child;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LevelNode<Cfg, level>::~LevelNode()
    {
        assert(isEmptyExceptFirst());
        if(_children[0])
        {
            delete _children[0];
            _useMask &= ~(1ull<<0);
        }
        assert(!_useMask);
        assert(!_fullMask);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LinkId LevelNode<Cfg, level>::add(Container *container, Link *link)
    {
        LinkId id = dci::utils::bits::least1Count(_fullMask);

        assert(id <= _width);
        if(id >= _width)
        {
            if(Parent::_level < _levels)
            {
                Parent *p = new Parent(this);
                container->levelUp(p, p->_level);
                return p->add(container, link);
            }

            return Cfg::_badLinkId;
        }

        assert(link && "null link added?");

        LinkId childId;

        if(!_children[id])
        {
            _children[id] = new Child();
            _useMask |= 1ull<<id;

            childId = _children[id]->add(link);
        }
        else
        {
            assert(!_children[id]->isFull());

            childId = _children[id]->add(link);

            if(_children[id]->isFull())
            {
                _fullMask |= 1ull<<id;
            }
        }

        return id * Child::_volume + childId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    LinkId LevelNode<Cfg, level>::add(Link *link)
    {
        LinkId id = dci::utils::bits::least1Count(_fullMask);

        assert(id <= _width);

        assert(link && "null link added?");

        LinkId childId;

        if(!_children[id])
        {
            _children[id] = new Child();
            _useMask |= 1ull<<id;

            childId = _children[id]->add(link);
        }
        else
        {
            assert(!_children[id]->isFull());

            childId = _children[id]->add(link);

            if(_children[id]->isFull())
            {
                _fullMask |= 1ull<<id;
            }
        }

        return id * Child::_volume + childId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool LevelNode<Cfg, level>::add(Container *container, LinkId id, Link *link)
    {
        auto cid = id / Child::_volume;

        if(cid >= _width)
        {
            if(Parent::_level < _levels)
            {
                Parent *p = new Parent(this);
                container->levelUp(p, p->_level);
                return p->add(container, id, link);
            }

            return Cfg::_badLinkId;
        }

        assert(link && "null link added?");

        if(!_children[cid])
        {
            _children[cid] = new Child();
            _useMask |= 1ull<<cid;

            return _children[cid]->add(id - cid*Child::_volume, link);
        }

        bool res = _children[cid]->add(id - cid*Child::_volume, link);

        if(_children[cid]->isFull())
        {
            _fullMask |= 1ull<<cid;
        }

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool LevelNode<Cfg, level>::add(LinkId id, Link *link)
    {
        auto cid = id / Child::_volume;

        assert(cid <= _width);

        assert(link && "null link added?");

        if(!_children[cid])
        {
            _children[cid] = new Child();
            _useMask |= 1ull<<cid;

            return _children[cid]->add(id - cid*Child::_volume, link);
        }

        bool res = _children[cid]->add(id - cid*Child::_volume, link);

        if(_children[cid]->isFull())
        {
            _fullMask |= 1ull<<cid;
        }

        return res;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename LevelNode<Cfg, level>::Link *LevelNode<Cfg, level>::get(const LinkId &id) const
    {
        auto cid = id / Child::_volume;
        if(cid < _width && _children[cid])
        {
            return _children[cid]->get(id - cid*Child::_volume);
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename LevelNode<Cfg, level>::Link *LevelNode<Cfg, level>::del(Container *container, const LinkId &id)
    {
        auto cid = id / Child::_volume;
        if(cid < _width && _children[cid])
        {
            _fullMask &= ~(1ull<<cid);

            Link *link = _children[cid]->del(id - cid*Child::_volume);
            if(cid && _children[cid]->isEmpty())
            {
                delete _children[cid];
                _children[cid] = nullptr;
                _useMask &= ~(1ull<<cid);

                if(isEmptyExceptFirst())
                {
                    Child *first = _children[0];
                    assert(first);

                    _children[0] = nullptr;
                    _useMask &= ~(1ull<<0);
                    _fullMask &= ~(1ull<<0);

                    first->probablyDown(container);

                    delete this;
                }
            }

            return link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename LevelNode<Cfg, level>::Link *LevelNode<Cfg, level>::del(const LinkId &id)
    {
        auto cid = id / Child::_volume;
        if(cid < _width && _children[cid])
        {
            _fullMask &= ~(1ull<<cid);

            Link *link = _children[cid]->del(id - cid*Child::_volume);

            if(cid && _children[cid]->isEmpty())
            {
                delete _children[cid];
                _children[cid] = nullptr;
                _useMask &= ~(1ull<<cid);
            }

            return link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    void LevelNode<Cfg, level>::probablyDown(Container *container)
    {
        if(isEmptyExceptFirst())
        {
            Child *first = _children[0];
            assert(first);

            _children[0] = nullptr;
            _useMask &= ~(1ull<<0);
            _fullMask &= ~(1ull<<0);

            first->probablyDown(container);

            delete this;
            return;
        }

        container->levelDown(this, level);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool LevelNode<Cfg, level>::isEmptyExceptFirst() const
    {
        return !(_useMask >> 1);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool LevelNode<Cfg, level>::isEmpty() const
    {
        if(_children[0] && !_children[0]->isEmpty())
        {
            return false;
        }

        return !(_useMask >> 1);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool LevelNode<Cfg, level>::isFull() const
    {
        return _fullFullMask == _fullMask;
    }
}}}
