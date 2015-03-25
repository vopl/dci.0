#include <ifaceState.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    IfaceState::IfaceState(Deleter deleter)
        : _refs{0}
        , _deleter{deleter}
    {
    }

    IfaceState::~IfaceState()
    {
        assert(!_refs);
    }

    void IfaceState::incRef()
    {
        _refs++;
    }

    void IfaceState::decRef()
    {
        _refs--;
        if(!_refs)
        {
            _deleter(this);
        }
    }

}}}
