#include <dci/async/barrier.hpp>
#include "impl/barrier.hpp"

namespace dci { namespace async
{
    Barrier::Barrier(std::size_t depth)
        : himpl::FaceLayout<impl::Barrier, Waitable>(depth)
    {
    }

    Barrier::~Barrier()
    {
    }

    void Barrier::traverse()
    {
        return impl().traverse();
    }

    bool Barrier::tryTraverse()
    {
        return impl().tryTraverse();
    }

    bool Barrier::canTraverse() const
    {
        return impl().canTraverse();
    }

}}
