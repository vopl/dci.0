#include "pulser.hpp"

namespace dci { namespace async { namespace impl
{
    Pulser::Pulser()
    {
    }

    Pulser::~Pulser()
    {
    }

//    bool Pulser::locked() const
//    {
//        return _locked;
//    }

//    void Pulser::lock()
//    {
//        assert(!_locked);
//    }

    void Pulser::pulse()
    {
        assert(_locked);
        _locked = false;

        Waitable::unlock();

        assert(!_locked);
        _locked = true;
    }
}}}
