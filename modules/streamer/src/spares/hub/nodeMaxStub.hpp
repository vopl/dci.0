#pragma once
#include "nodeBase.hpp"

namespace spares { namespace hub
{
    template <class Cfg>
    struct NodeMaxStub
        : public NodeBase<Cfg>
    {
        static const std::size_t _level = 100500;

        NodeMaxStub(void *)
        {
            assert(0&&"never here");
        }

        Id add(void *, void *)
        {
            assert(0&&"never here");
            return _badId;
        }

        bool add(void *, std::size_t, void *)
        {
            assert(0&&"never here");
            return false;
        }

    };

}}
