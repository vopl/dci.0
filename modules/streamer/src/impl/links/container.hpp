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
    class Container
    {

    private:
        using Link = Link_;
        struct Cfg
        {
            using Container = links::Container<Link_, volume>;
            using Link = Link_;
            static const std::size_t _width = bitsof(void*);
            static const Id _badLinkId = static_cast<Id>(-1);

            static constexpr std::size_t evelLevels(std::size_t width, std::size_t totalVolume, std::size_t sum = 1)
            {
                return sum >= totalVolume ? 0 : evelLevels(width, totalVolume, width * sum)+1;
            }

            static const std::size_t _levels = evelLevels(_width, volume, 1);
        };

    public:

        Container();
        ~Container();

        Link *add();
        Link *add(const Id &id);
        Link *get(const Id &id);
        std::unique_ptr<Link> del(const Id &id);


    private:

        template <class Cfg_, std::size_t level>
        friend class Node;

        void levelUp(NodeBase<Cfg> *node, std::size_t level);
        void levelDown(NodeBase<Cfg> *node, std::size_t level);


    private:
        std::size_t _currentLevel;
        NodeBase<Cfg> *_currentNode;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Container<Link, volume>::Container()
        : _currentLevel(0)
        , _currentNode(new Node<Cfg, 0>())
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Container<Link, volume>::~Container()
    {
        assert(_currentNode);
        _currentNode->destroy(_currentLevel);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Container<Link, volume>::add()
    {
        Link *link = new Link;
        Id id = _currentNode->add(_currentLevel, this, link);
        if(id < volume)
        {
            link->setId(static_cast<streamer::ServiceHub::ServiceId>(id));
            return link;
        }

        _currentNode->del(_currentLevel, this, id);
        delete link;
        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Container<Link, volume>::add(const Id &id)
    {
        if(id < volume)
        {
            Link *link = new Link;

            if(_currentNode->add(_currentLevel, this, id, link))
            {
                link->setId(static_cast<streamer::ServiceHub::ServiceId>(id));
                return link;
            }

            delete link;
        }

        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Container<Link, volume>::get(const Id &id)
    {
        assert(id < volume);
        return _currentNode->get(_currentLevel, id);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    std::unique_ptr<Link> Container<Link, volume>::del(const Id &id)
    {
        assert(id < volume);
        Link *link = _currentNode->del(_currentLevel, this, id);
        return std::unique_ptr<Link>(link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Container<Link, volume>::levelUp(NodeBase<Cfg> *node, std::size_t level)
    {
        assert(_currentLevel < level);
        _currentNode = node;
        _currentLevel = level;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Container<Link, volume>::levelDown(NodeBase<Cfg> *node, std::size_t level)
    {
        assert(_currentLevel > level);
        _currentNode = node;
        _currentLevel = level;
    }

}}
