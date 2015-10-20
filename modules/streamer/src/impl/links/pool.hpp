#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/utils/bits.hpp>

#include "node0.hpp"
#include "node.hpp"

#include "streamer.hpp"


#include <set>
#include <unordered_set>
#include <chrono>
#include <iostream>

namespace impl { namespace links
{
    using namespace dci::couple::runtime;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link_, std::size_t volume = 1ull<<31 >
    class Pool
    {

    private:
        using Link = Link_;
        struct Cfg
        {
            using Pool = links::Pool<Link_, volume>;
            using Link = Link_;
            static const std::size_t _width = bitsof(void*);

            static constexpr std::size_t evelLevels(std::size_t width, std::size_t totalVolume, std::size_t sum = 1)
            {
                return sum >= totalVolume ? 0 : evelLevels(width, totalVolume, width * sum)+1;
            }

            static const std::size_t _levels = evelLevels(_width, volume, 1);
        };

    public:

        Pool();
        ~Pool();

        void setIdPrefix(Id v);
        bool isPrefixSame(Id v);

        Link *add();
        Link *add(const Id &id);
        Link *get(const Id &id) const;
        std::unique_ptr<Link> del(const Id &id);

        bool isEmpty() const;

        template <class F>
        void clean(F &&f);


    private:

        template <class Cfg_, std::size_t level>
        friend class Node;

        void levelUp(NodeBase<Cfg> *node, std::size_t level);
        void levelDown(NodeBase<Cfg> *node, std::size_t level);


    private:
        std::size_t _currentLevel;
        NodeBase<Cfg> *_currentNode;
        Id _idPrefix;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Pool<Link, volume>::Pool()
        : _currentLevel(0)
        , _currentNode(new Node<Cfg, 0>())
        , _idPrefix(0)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Pool<Link, volume>::~Pool()
    {
        assert(_currentNode);
        _currentNode->destroy(_currentLevel);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Pool<Link, volume>::setIdPrefix(Id v)
    {
        assert(!(v>>1));
        _idPrefix = v;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    bool Pool<Link, volume>::isPrefixSame(Id v)
    {
        return (_idPrefix&1) == (v&1);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Pool<Link, volume>::add()
    {
        Link *link = new Link;
        Id id = _currentNode->add(_currentLevel, this, link);
        if(id < volume)
        {
            link->setId((id << 1) | _idPrefix);
            return link;
        }

        if(_badId != id)
        {
            _currentNode->del(_currentLevel, this, id);
        }
        delete link;
        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Pool<Link, volume>::add(const Id &id)
    {
        assert((id&1) == _idPrefix);
        Id internalId = id>>1;

        if(internalId < volume)
        {
            Link *link = new Link;

            if(_currentNode->add(_currentLevel, this, internalId, link))
            {
                link->setId(id);
                return link;
            }

            delete link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Pool<Link, volume>::get(const Id &id) const
    {
        assert((id&1) == _idPrefix);
        Id internalId = id>>1;
        assert(internalId < volume);
        return _currentNode->get(_currentLevel, internalId);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    std::unique_ptr<Link> Pool<Link, volume>::del(const Id &id)
    {
        assert((id&1) == _idPrefix);
        Id internalId = id>>1;
        Link *link = _currentNode->del(_currentLevel, this, internalId);
        return std::unique_ptr<Link>(link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    bool Pool<Link, volume>::isEmpty() const
    {
        return _currentNode->isEmpty(_currentLevel);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    template <class F>
    void Pool<Link, volume>::clean(F &&f)
    {
        return _currentNode->clean(_currentLevel, std::forward<F>(f));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Pool<Link, volume>::levelUp(NodeBase<Cfg> *node, std::size_t level)
    {
        assert(_currentLevel < level);
        _currentNode = node;
        _currentLevel = level;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Pool<Link, volume>::levelDown(NodeBase<Cfg> *node, std::size_t level)
    {
        assert(_currentLevel > level);
        _currentNode = node;
        _currentLevel = level;
    }

}}
