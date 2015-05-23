#include <dci/async/pulser.hpp>
#include "impl/pulser.hpp"

namespace dci { namespace async
{
    Pulser::Pulser()
        : himpl::FaceLayout<impl::Pulser>()
    {

    }

    Pulser::~Pulser()
    {

    }

    void Pulser::wait()
    {
        return impl().lock();
    }

    void Pulser::pulse()
    {
        return impl().pulse();
    }

}}
