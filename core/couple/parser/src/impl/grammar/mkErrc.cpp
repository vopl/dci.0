#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkErrc()
    {
        ////////////////////////////////////////////////////////////////////////////////
        errc =
            toks.kwerrc[qi::_val = phx::construct<Errc>(phx::new_<SErrc>())] >
            (
                name[phx::bind(&SErrc::name, deref(qi::_val)) = qi::_1] |
                error(+"errc name expected")
            ) >
            (toks.ocb | error(+"'{' expected")) >
            *(
                errcField[phx::push_back(phx::bind(&SErrc::fields, deref(qi::_val)), qi::_1)]
            ) >
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
