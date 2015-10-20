#pragma once
#include <dci/mm/newDelete.hpp>
#include "nodeMaxStub.hpp"

namespace impl { namespace links
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    class Node
        : public NodeBase<Cfg>
        , public dci::mm::NewDelete<Node<Cfg, level>>
    {
    public:
        using Parent = std::conditional_t<
            (level<Cfg::_levels),
            Node<Cfg, level+1>,
            NodeMaxStub<Cfg>>;

        using Child = Node<Cfg, level-1>;

        using Pool = typename Cfg::Pool;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;
        static const std::size_t _level = level;
        static const std::size_t _volume = _width * Child::_volume;

        using Mask = std::size_t;
        static const Mask _fullFullMask = (bitsof(Mask)==_width ? (~std::size_t(0)) : ((1ull<<_width) - 1));

    public:
        Node();
        Node(Child *child);
        ~Node();

        Id add(Pool *pool, Link *link);
        Id add(Link *link);

        bool add(Pool *pool, Id id, Link *link);
        bool add(Id id, Link *link);

        Link *get(const Id &id) const;
        Link *del(Pool *pool, const Id &id);
        Link *del(const Id &id);

        void probablyDown(Pool *pool);

        bool isEmptyExceptFirst() const;
        bool isEmpty() const;
        bool isFull() const;

        template <class F>
        void clean(F &&f);

    private:
        Id add2Children(std::size_t childIndex, Link *link);
        bool add2Children(std::size_t childIndex, Id childId, Link *link);

    private:
        Mask _useMask = 0;
        Mask _fullMask = 0;
        static_assert(bitsof(_fullMask) >= _width, "width must not be greater then _fullMask");
        Child *_children[_width] = {};
    };





    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    Node<Cfg, level>::Node()
        : NodeBase<Cfg>()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    Node<Cfg, level>::Node(Child *child)
        : NodeBase<Cfg>()
        , _useMask(1ull<<0)
        , _fullMask(0)
    {
        assert(child);
        _children[0] = child;
        if(child->isFull())
        {
            _fullMask |= 1ull<<0;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    Node<Cfg, level>::~Node()
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
    Id Node<Cfg, level>::add(Pool *pool, Link *link)
    {
        std::size_t childIndex = dci::utils::bits::least1Count(_fullMask);

        assert(childIndex <= _width);
        if(childIndex >= _width)
        {
            if(Parent::_level < _levels)
            {
                Parent *p = new Parent(this);
                pool->levelUp(p, p->_level);
                return p->add(pool, link);
            }

            return _badId;
        }

        return add2Children(childIndex, link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    Id Node<Cfg, level>::add(Link *link)
    {
        std::size_t childIndex = dci::utils::bits::least1Count(_fullMask);
        return add2Children(childIndex, link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool Node<Cfg, level>::add(Pool *pool, Id id, Link *link)
    {
        std::size_t childIndex = id / Child::_volume;

        if(childIndex >= _width)
        {
            if(Parent::_level < _levels)
            {
                Parent *p = new Parent(this);
                pool->levelUp(p, p->_level);
                return p->add(pool, id, link);
            }

            return _badId;
        }

        return add2Children(childIndex, id - childIndex*Child::_volume, link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool Node<Cfg, level>::add(Id id, Link *link)
    {
        std::size_t childIndex = id / Child::_volume;
        return add2Children(childIndex, id - childIndex*Child::_volume, link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename Node<Cfg, level>::Link *Node<Cfg, level>::get(const Id &id) const
    {
        std::size_t childIndex = id / Child::_volume;
        if(childIndex < _width && _children[childIndex])
        {
            return _children[childIndex]->get(id - childIndex*Child::_volume);
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename Node<Cfg, level>::Link *Node<Cfg, level>::del(Pool *pool, const Id &id)
    {
        std::size_t childIndex = id / Child::_volume;
        if(childIndex < _width && _children[childIndex])
        {
            _fullMask &= ~(1ull<<childIndex);

            Link *link = _children[childIndex]->del(id - childIndex*Child::_volume);
            if(childIndex && _children[childIndex]->isEmpty())
            {
                delete _children[childIndex];
                _children[childIndex] = nullptr;
                _useMask &= ~(1ull<<childIndex);

                if(isEmptyExceptFirst())
                {
                    Child *first = _children[0];
                    assert(first);

                    _children[0] = nullptr;
                    _useMask &= ~(1ull<<0);
                    _fullMask &= ~(1ull<<0);

                    first->probablyDown(pool);

                    delete this;
                }
            }

            return link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    typename Node<Cfg, level>::Link *Node<Cfg, level>::del(const Id &id)
    {
        std::size_t childIndex = id / Child::_volume;
        if(childIndex < _width && _children[childIndex])
        {
            _fullMask &= ~(1ull<<childIndex);

            Link *link = _children[childIndex]->del(id - childIndex*Child::_volume);

            if(childIndex && _children[childIndex]->isEmpty())
            {
                delete _children[childIndex];
                _children[childIndex] = nullptr;
                _useMask &= ~(1ull<<childIndex);
            }

            return link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    void Node<Cfg, level>::probablyDown(Pool *pool)
    {
        if(isEmptyExceptFirst())
        {
            Child *first = _children[0];
            assert(first);

            _children[0] = nullptr;
            _useMask &= ~(1ull<<0);
            _fullMask &= ~(1ull<<0);

            first->probablyDown(pool);

            delete this;
            return;
        }

        pool->levelDown(this, level);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool Node<Cfg, level>::isEmptyExceptFirst() const
    {
        return !(_useMask >> 1);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool Node<Cfg, level>::isEmpty() const
    {
        if(_children[0] && !_children[0]->isEmpty())
        {
            return false;
        }

        return !(_useMask >> 1);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool Node<Cfg, level>::isFull() const
    {
        return _fullFullMask == _fullMask;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    template <class F>
    void Node<Cfg, level>::clean(F &&f)
    {
        while(_useMask)
        {
            Id id = dci::utils::bits::least0Count(_useMask);
            assert(id<_width);
            assert(_children[id]);
            assert((_useMask>>id) & 1);

            _useMask &= ~(1ull<<id);
            _fullMask &= ~(1ull<<id);

            Child *child = _children[id];
            _children[id] = nullptr;

            child->clean(std::forward<F>(f));
            delete child;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    Id Node<Cfg, level>::add2Children(std::size_t childIndex, Link *link)
    {
        assert(childIndex <= _width);

        assert(link && "null link added?");

        Id childId;

        if(!_children[childIndex])
        {
            _children[childIndex] = new Child();
            _useMask |= 1ull<<childIndex;

            childId = _children[childIndex]->add(link);
        }
        else
        {
            assert(!_children[childIndex]->isFull());

            childId = _children[childIndex]->add(link);

            if(_children[childIndex]->isFull())
            {
                _fullMask |= 1ull<<childIndex;
            }
        }

        return childIndex * Child::_volume + childId;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg, std::size_t level>
    bool Node<Cfg, level>::add2Children(std::size_t childIndex, Id childId, Link *link)
    {
        assert(childIndex <= _width);

        assert(link && "null link added?");

        if(!_children[childIndex])
        {
            _children[childIndex] = new Child();
            _useMask |= 1ull<<childIndex;

            return _children[childIndex]->add(childId, link);
        }

        bool res = _children[childIndex]->add(childId, link);

        if(_children[childIndex]->isFull())
        {
            _fullMask |= 1ull<<childIndex;
        }

        return res;
    }

}}
