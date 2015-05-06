#include <dci/couple/runtime/ifaceWire.hpp>
#include <cassert>
#include <cstdlib>

namespace dci { namespace couple { namespace runtime
{
    IfaceWire::IfaceWire(Deleter deleter)
        : _involvedFwd{}
        , _involvedBwd{}
        , _uninvolvFwdListener{}
        , _uninvolvFwdListenerUserData{}
        , _uninvolvBwdListener{}
        , _uninvolvBwdListenerUserData{}
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
            if(_uninvolvFwdListener && !use)
            {
                _uninvolvFwdListener(_uninvolvFwdListenerUserData);
                _uninvolvFwdListener = nullptr;
                _uninvolvFwdListenerUserData = nullptr;
            }
            _involvedFwd = use;
        }
        else
        {
            assert(_involvedBwd != use);
            if(_uninvolvBwdListener && !use)
            {
                _uninvolvBwdListener(_uninvolvBwdListenerUserData);
                _uninvolvBwdListener = nullptr;
                _uninvolvBwdListenerUserData = nullptr;
            }
            _involvedBwd = use;
        }

        if(!_involvedFwd && !_involvedBwd)
        {
            _deleter(this);
        }
    }

    void IfaceWire::listenUninvolve(bool fwd, UninvolveListener listener, void *userData)
    {
        if(fwd)
        {
            if(_uninvolvFwdListener && listener)
            {
                assert("_uninvolvFwdListener already setted");
                abort();
                return;
            }

            _uninvolvFwdListener = listener;
            _uninvolvFwdListenerUserData = userData;
            return;
        }

        if(_uninvolvBwdListener && listener)
        {
            assert("_uninvolvBwdListener already setted");
            abort();
            return;
        }

        _uninvolvBwdListener = listener;
        _uninvolvBwdListenerUserData = userData;
    }

}}}
