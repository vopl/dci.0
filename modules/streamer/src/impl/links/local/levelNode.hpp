#pragma once
#include "levelNodeBase.hpp"

namespace impl { namespace links { namespace local
{

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels, std::size_t level>
    class LevelNode
        : public LevelNodeBase<Link, width, levels>
    {
        using Parent = LevelNode<Link, width, levels, level+1>;
        using Child = LevelNode<Link, width, levels, level-1>;

    public:
        LevelNode(LinkId idOffset);
        ~LevelNode();

        LinkId add(Local<Link> *container, std::unique_ptr<Link> &&link);
        Link *get(const LinkId &id);
        std::unique_ptr<Link> del(Local<Link> *container, const LinkId &id);

    private:
        using LevelNodeBase<Link, width, levels>::_idOffset;

    private:
        std::unique_ptr<Child> _children[width] = {};
    };










    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels, std::size_t level>
    LevelNode<Link, width, levels, level>::LevelNode(LinkId idOffset)
        : LevelNodeBase<Link, width, levels>(idOffset)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels, std::size_t level>
    LevelNode<Link, width, levels, level>::~LevelNode()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels, std::size_t level>
    LinkId LevelNode<Link, width, levels, level>::add(Local<Link> *container, std::unique_ptr<Link> &&link)
    {
        (void)container;
        (void)link;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels, std::size_t level>
    Link *LevelNode<Link, width, levels, level>::get(const LinkId &id)
    {
        (void)id;
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Link, std::size_t width, std::size_t levels, std::size_t level>
    std::unique_ptr<Link> LevelNode<Link, width, levels, level>::del(Local<Link> *container, const LinkId &id)
    {
        (void)container;
        (void)id;
        assert(0);
    }

}}}
