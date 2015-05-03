#include <dci/couple/runtime/ifaceWire.hpp>
#include <cassert>

namespace dci { namespace couple { namespace runtime
{
    IfaceWire::IfaceWire(Deleter deleter)
        : _involvedFwd{}
        , _involvedBwd{}
        , _deleter{deleter}
    {
    }

    IfaceWire::~IfaceWire()
    {
        assert(!_involvedFwd && !_involvedBwd);
    }


    void IfaceWire::involve(bool fwd, bool use)
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
