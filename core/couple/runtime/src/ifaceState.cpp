#include <ifaceState.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    IfaceState::IfaceState(Deleter deleter)
        : _involvedFwd{}
        , _involvedBwd{}
        , _deleter{deleter}
    {
    }

    IfaceState::~IfaceState()
    {
        assert(!_involvedFwd && !_involvedBwd);
    }


    void IfaceState::involve(bool fwd, bool use)
    {
        if(fwd)
        {
            assert(_involvedFwd != use);
            _involvedFwd = use;
        }
        else
        {
            assert(_involvedBwd != use);
            _involvedBwd = use;
        }

        if(!_involvedFwd && !_involvedBwd)
        {
            _deleter(this);
        }
    }

}}}
