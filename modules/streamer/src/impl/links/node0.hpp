#pragma once
#include "nodeBase.hpp"
#include <dci/utils/bits.hpp>

namespace impl { namespace links
{
    template <class Cfg>
    class Node<Cfg, 0>
        : public NodeBase<Cfg>
        , public dci::mm::NewDelete<Node<Cfg, 0>>
    {
    public:
        using Parent = Node<Cfg, 1>;

        using Pool = typename Cfg::Pool;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;
        static const std::size_t _level = 0;
        static const std::size_t _volume = _width * 1;

        using Mask = std::size_t;
        static const Mask _fullUseMask = (bitsof(Mask)==_width ? (~std::size_t(0)) : ((1ull<<_width) - 1));

    public:
        Node();
        ~Node();

        Id add(Pool *pool, Link *link);
        Id add(Link *link);

        bool add(Pool *pool, Id id, Link *link);
        bool add(Id id, Link *link);

        Link *get(const Id &id) const;

        Link *del(Pool *pool, const Id &id);
        Link *del(const Id &id);

        void probablyDown(Pool *pool);

        bool isEmpty() const;
        bool isFull() const;

        void clean();

        template <class F>
        void clean(F &&f);

    private:
        Mask _useMask = 0;
        static_assert(bitsof(_useMask) >= _width, "width must not be greater then _useMask");
        Link *_links[_width] = {};
    };










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    Node<Cfg, 0>::Node()
        : NodeBase<Cfg>()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    Node<Cfg, 0>::~Node()
    {
        assert(!_useMask);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    Id Node<Cfg, 0>::add(Pool *pool, Link *link)
    {
        Id id = dci::utils::bits::least1Count(_useMask);

        assert(id <= _width);
        if(id >= _width)
        {
            if(Parent::_level < _levels)
            {
                Parent *p = new Parent(this);
                pool->levelUp(p, p->_level);
                return p->add(link);
            }

            return _badId;
        }

        assert(link && "null link added?");
        assert(!_links[id]);
        _links[id] = link;
        _useMask |= 1ull<<id;

        return id;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    Id Node<Cfg, 0>::add(Link *link)
    {
        Id id = dci::utils::bits::least1Count(_useMask);

        assert(id <= _width);

        assert(link && "null link added?");
        assert(!_links[id]);
        _links[id] = link;
        _useMask |= 1ull<<id;

        return id;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool Node<Cfg, 0>::add(Pool *pool, Id id, Link *link)
    {
        if(id >= _width)
        {
            if(Parent::_level < _levels)
            {
                Parent *p = new Parent(this);
                pool->levelUp(p, p->_level);
                return p->add(pool, id, link);
            }

            return false;
        }

        if(_links[id])
        {
            return false;
        }

        assert(link && "null link added?");
        _links[id] = link;
        _useMask |= 1ull<<id;
        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool Node<Cfg, 0>::add(Id id, Link *link)
    {
        assert(id < _width);

        if(_links[id])
        {
            return false;
        }

        assert(link && "null link added?");
        _links[id] = link;
        _useMask |= 1ull<<id;
        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename Node<Cfg, 0>::Link *Node<Cfg, 0>::get(const Id &id) const
    {
        if(id < _width && _links[id])
        {
            assert((_useMask>>id) & 1);
            return _links[id];
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename Node<Cfg, 0>::Link *Node<Cfg, 0>::del(Pool *pool, const Id &id)
    {
        (void)pool;
        return del(id);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename Node<Cfg, 0>::Link *Node<Cfg, 0>::del(const Id &id)
    {
        if(id < _width && _links[id])
        {
            assert((_useMask>>id) & 1);

            _useMask &= ~(1ull<<id);
            Link *link = _links[id];
            _links[id] = nullptr;
            return link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    void Node<Cfg, 0>::probablyDown(Pool *pool)
    {
        pool->levelDown(this, _level);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool Node<Cfg, 0>::isEmpty() const
    {
        return !_useMask;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool Node<Cfg, 0>::isFull() const
    {
        return _fullUseMask == _useMask;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    template <class F>
    void Node<Cfg, 0>::clean(F &&f)
    {
        while(_useMask)
        {
            Id id = dci::utils::bits::least0Count(_useMask);
            assert(id<_width);
            assert(_links[id]);
            assert((_useMask>>id) & 1);

            _useMask &= ~(1ull<<id);
            Link *link = _links[id];
            _links[id] = nullptr;

            f(std::unique_ptr<Link>(link));
        }
    }


}}
