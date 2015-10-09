#pragma once
#include "levelNodeBase.hpp"

namespace impl { namespace links { namespace local
{
    template <class Link, std::size_t width, std::size_t levels>
    class LevelNode<Link, width, levels, 0>
        : public LevelNodeBase<Link, width, levels>
    {
        using Parent = LevelNode<Link, width, levels, 1>;

    public:
        LevelNode(const LinkId &idOffset);
        ~LevelNode();

        LinkId add(Local<Link> *container, std::unique_ptr<Link> &&link);
        Link *get(const LinkId &id);
        std::unique_ptr<Link> del(Local<Link> *container, const LinkId &id);

    private:
        using LevelNodeBase<Link, width, levels>::_idOffset;

    private:
        int _useMask = {};
        std::unique_ptr<Link> _links[width] = {};
    };










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    LevelNode<Link, width, levels, 0>::LevelNode(const LinkId &idOffset)
        : LevelNodeBase<Link, width, levels>(idOffset)
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    LevelNode<Link, width, levels, 0>::~LevelNode()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    LinkId LevelNode<Link, width, levels, 0>::add(Local<Link> *container, std::unique_ptr<Link> &&link)
    {
        for(std::size_t i(0); i<width; ++i)
        {
            if(!_links[i])
            {
                _links[i] = std::move(link);

                LinkId id = _idOffset + i;
                return id;
            }
        }

        return container->levelUpAdd(std::move(link));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    Link *LevelNode<Link, width, levels, 0>::get(const LinkId &id)
    {
        assert(id >= _idOffset && id < _idOffset+width);
        assert(_links[id-_idOffset]);
        return _links[id-_idOffset].get();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels>
    std::unique_ptr<Link> LevelNode<Link, width, levels, 0>::del(Local<Link> *container, const LinkId &id)
    {
        assert(id >= _idOffset && id < _idOffset+width);
        assert(_links[id-_idOffset]);

        (void)container;
        assert(!"remove empty nodes");

        return std::move(_links[id-_idOffset]);
    }

}}}
