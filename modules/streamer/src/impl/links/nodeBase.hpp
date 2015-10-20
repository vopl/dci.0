#pragma once
#include <dci/couple/runtime.hpp>
#include <dci/utils/switchExpander.hpp>
#include "id.hpp"

namespace impl { namespace links
{
    template <class Cfg, std::size_t level> class Node;

    template <class Cfg> class NodeBase;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    class NodeBase
    {
        NodeBase(const NodeBase &) = delete;
        NodeBase &operator=(const NodeBase &) = delete;

    public:
        using Pool = typename Cfg::Pool;
        using Link = typename Cfg::Link;
        static const std::size_t _width = Cfg::_width;
        static const std::size_t _levels = Cfg::_levels;

    protected:
        NodeBase();
        ~NodeBase();

    public:
        static NodeBase *create(std::size_t level, NodeBase *child);
        void destroy(std::size_t level);

        Id add(std::size_t level, Pool *pool, Link *link);
        bool add(std::size_t level, Pool *pool, Id id, Link *link);
        Link *get(std::size_t level, const Id &id) const;
        Link *del(std::size_t level, Pool *pool, const Id &id);

        bool isEmpty(std::size_t level) const;

        template <class F>
        void clean(std::size_t level, F &&f);

    private:
        template <class F>
        static auto expandSwitch(std::size_t level, F &&f);
    };













    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    NodeBase<Cfg>::NodeBase()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    NodeBase<Cfg>::~NodeBase()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    NodeBase<Cfg> *NodeBase<Cfg>::create(std::size_t level, NodeBase *child)
    {
        assert(level > 0);
        return expandSwitch(level-1, [&](auto vholder){

            using Child = Node<Cfg, vholder.value>;
            using This = Node<Cfg, vholder.value+1>;

            Child *concreteChild = static_cast<Child *>(child);
            return static_cast<NodeBase<Cfg> *>(new This(concreteChild));
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    void NodeBase<Cfg>::destroy(std::size_t level)
    {
        expandSwitch(level, [&](auto vholder){
            delete static_cast<Node<Cfg, vholder.value>*>(this);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    Id NodeBase<Cfg>::add(std::size_t level, Pool *pool, Link *link)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<Node<Cfg, vholder.value>*>(this)->add(pool, link);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool NodeBase<Cfg>::add(std::size_t level, Pool *pool, Id id, Link *link)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<Node<Cfg, vholder.value>*>(this)->add(pool, id, link);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename NodeBase<Cfg>::Link *NodeBase<Cfg>::get(std::size_t level, const Id &id) const
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<const Node<Cfg, vholder.value>*>(this)->get(id);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    typename NodeBase<Cfg>::Link *NodeBase<Cfg>::del(std::size_t level, Pool *pool, const Id &id)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<Node<Cfg, vholder.value>*>(this)->del(pool, id);
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    bool NodeBase<Cfg>::isEmpty(std::size_t level) const
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<const Node<Cfg, vholder.value>*>(this)->isEmpty();
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    template <class F>
    void NodeBase<Cfg>::clean(std::size_t level, F &&f)
    {
        return expandSwitch(level, [&](auto vholder){
            return static_cast<Node<Cfg, vholder.value>*>(this)->clean(std::forward<F>(f));
        });
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class Cfg>
    template <class F>
    auto NodeBase<Cfg>::expandSwitch(std::size_t level, F &&f)
    {
        return dci::utils::SwitchExpander<std::make_index_sequence<_levels>>::exec(level, std::move(f));
    }


}}
