#include <dci/async/waiter.hpp>
#include "impl/waiter.hpp"

namespace dci { namespace async
{

    Waiter::Waiter(WWLink *links, std::size_t amount)
        : himpl::FaceLayout<impl::Waiter>(links, amount)
    {
    }

    Waiter::~Waiter()
    {
    }

    std::size_t Waiter::any()
    {
        return impl().any();
    }

    void Waiter::all()
    {
        return impl().all();
    }

    void Waiter::each()
    {
        return impl().each();
    }
}}
