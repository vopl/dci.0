#include <dci/async/pulserAll.hpp>
#include "impl/pulserAll.hpp"

namespace dci { namespace async
{
    PulserAll::PulserAll()
        : himpl::FaceLayout<impl::PulserAll, Waitable>()
    {
    }

    PulserAll::~PulserAll()
    {
    }

    void PulserAll::acquire()
    {
        return impl().acquire();
    }

    void PulserAll::pulse()
    {
        return impl().pulse();
    }
}}
