#include <dci/async/waitable.hpp>
#include "impl/waitable.hpp"

namespace dci { namespace async
{
    Waitable::Waitable()
        : himpl::FaceLayout<impl::Waitable>(nullptr, nullptr)
    {
        assert(0&&"never here");
    }

    Waitable::~Waitable()
    {
    }

    void Waitable::wait()
    {
        return impl().wait();
    }

}}
