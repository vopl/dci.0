#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/utils/bits.hpp>

#include "local/levelNode0.hpp"
#include "local/levelNode.hpp"

#include "streamer.hpp"
#include <boost/pool/pool_alloc.hpp>




#include <set>
#include <unordered_set>
#include <chrono>
#include <iostream>

namespace impl { namespace links
{
    using namespace dci::couple::runtime;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link_, std::size_t volume = 1ull<<31 >
    class Local
    {

    private:
        using Link = Link_;
        struct Cfg
        {
            using Container = Local;
            using Link = Link_;
            static const std::size_t _width = bitsof(void*);
            static const std::size_t _badLinkId = -1;

            static constexpr std::size_t evelLevels(std::size_t width, std::size_t totalVolume, std::size_t sum = 1)
            {
                return sum >= totalVolume ? 0 : evelLevels(width, totalVolume, width * sum)+1;
            }

            static const std::size_t _levels = evelLevels(_width, volume, 1);
        };

    public:

        Local();
        ~Local();

        Link *add();
        Link *add(const local::LinkId &id);
        Link *get(const local::LinkId &id);
        std::unique_ptr<Link> del(const local::LinkId &id);


    private:

        template <class Cfg_, std::size_t level>
        friend class local::LevelNode;

        void levelUp(local::LevelNodeBase<Cfg> *node, std::size_t level);
        void levelDown(local::LevelNodeBase<Cfg> *node, std::size_t level);


    private:
        std::size_t _currentLevel;
        local::LevelNodeBase<Cfg> *_currentLevelNode;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Local<Link, volume>::Local()
        : _currentLevel(0)
        , _currentLevelNode(new local::LevelNode<Cfg, 0>())
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Local<Link, volume>::~Local()
    {
        assert(_currentLevelNode);
        _currentLevelNode->destroy(_currentLevel);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Local<Link, volume>::add()
    {
        Link *link = new Link;
        local::LinkId id = _currentLevelNode->add(_currentLevel, this, link);
        if(id < volume)
        {
            link->setId(id);
            return link;
        }

        _currentLevelNode->del(_currentLevel, this, id);
        delete link;
        return nullptr;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    Link *Local<Link, volume>::add(const local::LinkId &id)
    {
        if(id < volume)
        {
            Link *link = new Link;

            if(_currentLevelNode->add(_currentLevel, this, id, link))
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
    Link *Local<Link, volume>::get(const local::LinkId &id)
    {
        assert(id < volume);
        return _currentLevelNode->get(_currentLevel, id);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    std::unique_ptr<Link> Local<Link, volume>::del(const local::LinkId &id)
    {
        assert(id < volume);
        Link *link = _currentLevelNode->del(_currentLevel, this, id);
        return std::unique_ptr<Link>(link);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Local<Link, volume>::levelUp(local::LevelNodeBase<Cfg> *node, std::size_t level)
    {
        assert(_currentLevel < level);
        _currentLevelNode = node;
        _currentLevel = level;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t volume>
    void Local<Link, volume>::levelDown(local::LevelNodeBase<Cfg> *node, std::size_t level)
    {
        assert(_currentLevel > level);
        _currentLevelNode = node;
        _currentLevel = level;
    }

}}
