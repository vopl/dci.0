#include <ifaceState.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    IfaceState::IfaceState(Deleter deleter)
    {
        assert(0);
    }

    IfaceState::~IfaceState()
    {
        assert(0);
    }

    void IfaceState::incRef()
    {
        assert(0);
    }

    void IfaceState::decRef()
    {
        assert(0);
    }


    IfaceStatePtr::IfaceStatePtr()
    {
        assert(0);
    }

    IfaceStatePtr::IfaceStatePtr(IfaceState *state)
    {
        assert(0);
    }

    IfaceStatePtr::IfaceStatePtr(const IfaceStatePtr &from)
    {
        assert(0);
    }

    IfaceStatePtr::IfaceStatePtr(IfaceStatePtr &&from)
    {
        assert(0);
    }

    IfaceStatePtr::~IfaceStatePtr()
    {
        assert(0);
    }

    IfaceStatePtr IfaceStatePtr::operator=(const IfaceStatePtr &from)
    {
        assert(0);
    }

    IfaceStatePtr IfaceStatePtr::operator=(IfaceStatePtr &&from)
    {
        assert(0);
    }

}}}
