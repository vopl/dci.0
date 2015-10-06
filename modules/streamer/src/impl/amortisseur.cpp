#include "amortisseur.hpp"

namespace impl
{

    template <class Derived>
    Future< > Amortisseur<Derived>::startAmortisseur(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    template <class Derived>
    Future< > Amortisseur<Derived>::pauseAmortisseur()
    {
        assert(0);
    }

    template <class Derived>
    Future< > Amortisseur<Derived>::resumeAmortisseur()
    {
        assert(0);
    }

    template <class Derived>
    Future< Channel> Amortisseur<Derived>::migrateAmortisseur(Channel &&arg_0)
    {
        (void)arg_0;
        assert(0);
    }

    template <class Derived>
    Future< Channel> Amortisseur<Derived>::stopAmortisseur()
    {
        assert(0);
    }

    template <class Derived>
    Future< ::streamer::Amortisseur::AmortisseurState> Amortisseur<Derived>::amortisseurState()
    {
        assert(0);
    }
}

#include "../handlers/amortisseur.hpp"
template class impl::Amortisseur<handlers::Amortisseur>;
