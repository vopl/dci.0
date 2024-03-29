#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkAlias()
    {
        ////////////////////////////////////////////////////////////////////////////////
        alias =
            toks.kwalias[qi::_val = phx::construct<Alias>(phx::new_<SAlias>())] >
            (
                name[phx::bind(&SAlias::name, deref(qi::_val)) = qi::_1] |
                error(+"alias name expected")
            ) >
            (toks.eq | error(+"'=' expected")) >
            (
                typeUse[phx::bind(&SAlias::type, deref(qi::_val)) = qi::_1] |
                error(+"alias value expected")
            ) >
            (toks.semi | error(+"';' expected"));
    }

}}}}
