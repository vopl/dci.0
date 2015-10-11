#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/utils/bits.hpp>

#include "local/levelNode0.hpp"
#include "local/levelNode.hpp"
#include "local/levelNodeMaxStub.hpp"


#include <boost/pool/pool_alloc.hpp>




#include <set>
#include <unordered_set>
#include <chrono>
#include <iostream>

namespace impl { namespace links
{
    using namespace dci::couple::runtime;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link_>
    class Local
    {

    private:
        using Link = Link_;
        struct Cfg
        {
            using Container = Local;
            using Link = Link_;
            static const std::size_t _width = bitsof(void*);

            static constexpr std::size_t evelLevels(std::size_t width, std::size_t totalVolume, std::size_t sum = 1)
            {
                return sum >= totalVolume ? 0 : evelLevels(width, totalVolume, width * sum)+1;
            }

            static const std::size_t _levels = evelLevels(_width, 1ull<<31);
        };

    public:

        Local();
        ~Local();

        Link *add();
        Future<Interface> del(const local::LinkId &id);


    private:

        template <class Cfg_, std::size_t level>
        friend class local::LevelNode;

        local::LinkId levelUpAdd(std::unique_ptr<Link> &&link);
        void levelDown(local::LevelNodeBasePtr<Cfg> &&child);


    private:
        std::size_t _currentLevel;
        local::LevelNodeBasePtr<Cfg> _currentLevelNode;

        std::uint8_t _serviceIdScopeMark = 0;//0 or 1, for first id bit in ServiceId
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Local<Link>::Local()
        : _currentLevel(0)
        , _currentLevelNode(new local::LevelNode<Cfg, 0>())
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Local<Link>::~Local()
    {
        _currentLevelNode.release()->destroy(_currentLevel);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Link *Local<Link>::add()
    {



        if(1)
        {
            using set = std::unordered_set<
                    Link *,
                    std::hash<Link *>,
                    std::equal_to<Link *>,
                    boost::fast_pool_allocator<
                        Link*,
                        boost::default_user_allocator_new_delete,
                        boost::details::pool::null_mutex>>;

            set &slinks = *(new set);

            auto start = std::chrono::high_resolution_clock::now();
            for(std::size_t x(0); x<4000000; ++x)
            {
                Link *link = new Link;
                slinks.insert(link);
                link->setId(220);
            }

            std::cout<< (std::chrono::high_resolution_clock::now() - start).count()<<std::endl;
        }

        if(1)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(std::size_t x(0); x<4000000; ++x)
            {
                Link *link = new Link;
                local::LinkId id = _currentLevelNode->add(_currentLevel, this, std::unique_ptr<Link>(link));
                link->setId(id<<1 | _serviceIdScopeMark);
            }

            std::cout<< (std::chrono::high_resolution_clock::now() - start).count()<<std::endl;
        }

        exit(0);

        Link *link = new Link;
        local::LinkId id = _currentLevelNode->add(_currentLevel, this, std::unique_ptr<Link>(link));
        link->setId(id<<1 | _serviceIdScopeMark);
        return link;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    Future<Interface> Local<Link>::del(const local::LinkId &id)
    {
        std::unique_ptr<Link> link = _currentLevelNode->del(_currentLevel, this, id);
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
        if(_currentLevel >= Cfg::_levels)
        {
            assert(!"no more space left. What now? Handle error");
            abort();
        }

        _currentLevel++;
        _currentLevelNode = local::LevelNodeBase<Cfg>::create(_currentLevel, std::move(_currentLevelNode));

        return _currentLevelNode->add(_currentLevel, this, std::move(link));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link>
    void Local<Link>::levelDown(local::LevelNodeBasePtr<Cfg> &&child)
    {
        (void)child;
        assert(0);
    }

}}
