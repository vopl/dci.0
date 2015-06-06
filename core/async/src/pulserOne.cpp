#include <dci/async/pulserOne.hpp>
#include "impl/pulserOne.hpp"

namespace dci { namespace async
{
    PulserOne::PulserOne()
        : himpl::FaceLayout<impl::PulserOne, Waitable>()
    {
    }

    PulserOne::~PulserOne()
    {
    }

    void PulserOne::acquire()
    {
        return impl().acquire();
    }

    void PulserOne::pulse()
    {
        return impl().pulse();
    }
}}
