#pragma once

#include <dci/couple/runtime.hpp>

#include "local/levelNode0.hpp"
#include "local/levelNode.hpp"
#include "local/levelNodeMaxStub.hpp"

namespace impl { namespace links
{
    using namespace dci::couple::runtime;



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    class Local
    {

    public:

        Local();
        ~Local();

        Link *add();
        Future<Interface> del(const local::LinkId &id);


    private:

        static const std::size_t _width = 7;
        static const std::size_t _levels = 8;

        template <class Link_, std::size_t width, std::size_t levels, std::size_t level>
        friend class local::LevelNode;

        local::LinkId levelUpAdd(std::unique_ptr<Link> &&link);
        void levelDown(local::LevelNodeBasePtr<Link, _width, _levels> &&child);

    private:
        std::size_t _currentLevel;
        local::LevelNodeBasePtr<Link, _width, _levels> _levelNode;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Local<Link>::Local()
        : _currentLevel(0)
        , _levelNode(new local::LevelNode<Link, _width, _levels, 0>(0))
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Local<Link>::~Local()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Link *Local<Link>::add()
    {
        Link *link = new Link;
        local::LinkId id = _levelNode->add(_currentLevel, this, std::unique_ptr<Link>(link));
        link->setId(id);
        return link;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Future<Interface> Local<Link>::del(const local::LinkId &id)
    {
        std::unique_ptr<Link> link = _levelNode->del(_currentLevel, this, id);
        if(!link)
        {
            return Future<Interface>(make_error_code(streamer::error::badServiceId));
        }

        return link->shutdown();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    local::LinkId Local<Link>::levelUpAdd(std::unique_ptr<Link> &&link)
    {
        (void)link;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    void Local<Link>::levelDown(local::LevelNodeBasePtr<Link, _width, _levels> &&child)
    {
        (void)child;
        assert(0);
    }

}}
